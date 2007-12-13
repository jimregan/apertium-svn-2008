require 'libglade2'

class MainWindow

  def initialize(path)
    @glade = GladeXML.new(path) {|handler| method(handler)}
    @app_window = @glade.get_widget("app_window")
  end
end

Gnome::Program.new('Apertium viewer', "0.1")
MainWindow.new("main_window.glade")
Gtk.main
    
