require 'libglade2'
require 'gconf2'
require 'dbus'
require 'thread'
require 'base64'

class GladeXML
  def dump_gtk_state(directory, dict)
    self.widget_names.each do |name|
      begin
      self[name].dump_state.each_pair do |key, val|
        dict[directory + "/" + name + "." + key] = val.to_s
      end
      rescue
      end
    end
  end

  def load_gtk_state(directory, dict)
    widget_props = {}
    self.widget_names.each do |name| widget_props[name] = {} end

    dict.all_entries(directory).each do |entry|
      begin
        widget_name, prop_name = entry.key[/[a-zA-Z0-9_]*[.][a-zA-Z0-9_]*/].split(".")
        widget_props[widget_name][prop_name] = entry.value
      rescue
        # Deal with badly written data
      end
    end

    widget_props.each_pair do |key, val|
      begin
        self[key].load_state(val)
      rescue
        # bummer
      end
    end
  end
end

class Gtk::Widget
  def dump_state
    {}
  end

  def load_state(p)
  end
end

class Gtk::Window
  def dump_state
    { "x_size" => self.size[0],
      "y_size" => self.size[1],
      "x_pos"  => self.position[0], 
      "y_pos"  => self.position[1] }
  end

  def load_state(p)
    self.move(p["x_pos"].to_i, p["y_pos"].to_i)
    self.resize(p["x_size"].to_i, p["y_size"].to_i)
  end
end

class Gtk::Paned
  def dump_state
    { "position" => self.position }
  end

  def load_state(p)
    self.position = p["position"].to_i
  end
end

class MainWindow
  @@PROP_DIRECTORY = "/apps/aper/simple_viewer"

  def setup_combo(info, combo)
    combo.set_model(Gtk::ListStore.new(String))
    cell = Gtk::CellRendererText.new()
    combo.pack_start(cell, true)
    combo.add_attribute(cell, "text", 0)

    @info.modes.each do |mode|
      combo.append_text(mode)
    end
    combo.set_active(0)

    combo
  end

  def options
    if @chkMarkUnknown.active?
      {"mark_unknown" => "true"}
    else
      {}
    end
  end

  def initialize(path)
    @client = GConf::Client.default
    @bus = DBus::SessionBus.instance

    @info = @bus.introspect("org.apertium.info", "/")["org.apertium.Info"]
    @mark_unknown = false

    @input_queue  = Queue.new # Thread communication queues

    @glade = GladeXML.new(path) {|handler| method(handler)}

    @buffer         = @glade["txtInput"].buffer
    @output_buffer  = @glade["txtOutput"].buffer
    @wndMain        = @glade["wndMain"]
    @dlgAbout       = @glade["dlgAbout"]
    @chkMarkUnknown = @glade["chkMarkUnknown"]

    @comboPair      = self.setup_combo(@info, @glade["comboPair"])

    @buffer.signal_connect("changed") do |w|
      @input_queue.push([self.options, w.get_text(w.start_iter, w.end_iter)])
    end

    Thread.new do
      loop do
        options, input = @input_queue.pop # Block until we have something
        options, input = @input_queue.pop until @input_queue.empty? # Then pop the queue until we have the latest item
        begin
          result = @translator.translate(options, @buffer.get_text(@buffer.start_iter, @buffer.end_iter))
        rescue Exception => e
          puts e
        end

        Gtk.idle_add do # Queue a buffer in GTK's main loop update
          @output_buffer.set_text(result)
          false
        end
      end
    end

    @glade.load_gtk_state(@@PROP_DIRECTORY, @client)
    @wndMain.visible = true
  end

  def on_btnQuit_clicked(*widget)
    @glade.dump_gtk_state(@@PROP_DIRECTORY, @client)
    Gtk.main_quit
  end

  def on_wndMain_destroy(widget)
    Gtk.main_quit
  end

  def on_wndMain_delete_event(widget, event)
    @glade.dump_gtk_state(@@PROP_DIRECTORY, @client)
    false
  end

  def on_btnAbout_clicked(widget)
    @dlgAbout.show
  end

  def on_dlgAbout_response(widget, resp)
    @dlgAbout.hide
  end

  def on_comboPair_changed(widget)
    pair_name = widget.model.get_value(widget.active_iter, 0)
    dbus_pair_name = "/" + pair_name.split("-").join("_")
    @translator = @bus.introspect("org.apertium.mode", dbus_pair_name)["org.apertium.Mode"]
    @buffer.signal_emit("changed") 
  end

  def on_chkMarkUnknown_toggled(widget)
    @buffer.signal_emit("changed") 
  end
end

Gdk::Threads.init
Gnome::Program.new('Apertium viewer', "0.2")
MainWindow.new("main_window.glade")
Gtk.main
    
