import gtk.glade
from ConfigParser import DuplicateSectionError, NoSectionError
from widget_state import dump_state, load_state

class GladeXML(gtk.glade.XML):
    def get_widgets(self):
        return ((gtk.glade.get_widget_name(w), w) for w in  self.get_widget_prefix(''))

    
    def dump_gtk_state(self, cfg):
        for widget_name, widget in self.get_widgets():
            for key, val in dump_state(widget).iteritems():
                try:
                    cfg.add_section(widget_name)
                except DuplicateSectionError, e:
                    pass
                
                cfg.set(widget_name, key, str(val))


    def load_gtk_state(self, cfg):
        for widget_name, widget in self.get_widgets():
            try:
                load_state(widget, dict(cfg.items(widget_name)))
            except NoSectionError, e:
                pass


    def connect(self, context):
        handlers = dict((name, getattr(context, name)) for name in context.__class__.__dict__)

        self.signal_autoconnect(handlers)
