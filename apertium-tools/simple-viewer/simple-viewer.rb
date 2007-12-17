require 'libglade2'
require 'dbus'

class MainWindow
  def setup_combo(combo)
    combo.set_model(Gtk::ListStore.new(String))
    cell = Gtk::CellRendererText.new()
    combo.pack_start(cell, true)
    combo.add_attribute(cell, "text", 0)
    combo
  end

  def initialize(path)
    @glade = GladeXML.new(path) {|handler| method(handler)}
    @app_window = @glade.get_widget("app_window")
    @bus = DBus::SessionBus.instance

    @info = @bus.introspect("org.apertium.info", "/")["org.apertium.Info"]
    @translate = @bus.introspect("org.apertium.mode", "/")["org.apertium.Mode"]

    @comboPair = self.setup_combo(@glade["comboPair"])
    @info.modes.each do |mode|
      @comboPair.append_text(mode)
    end
    @comboPair.set_active(0)

    @buffer = @glade["txtInput"].buffer
    @buffer.signal_connect("changed") do |w|
      

      if @user_action
        @undopool <<  ["insert_text", iter.offset, iter.offset + text.scan(/./).size, text]
        @redopool.clear
      end
    end
  end

  def on_btnQuit_clicked(*widget)
    Gtk.main_quit
  end

  def on_wndMain_destroy(widget)
    Gtk.main_quit
  end

  def on_wndMain_delete_event(widget, event)
    false
  end
end

Gnome::Program.new('Apertium viewer', "0.1")
MainWindow.new("main_window.glade")
Gtk.main
    
