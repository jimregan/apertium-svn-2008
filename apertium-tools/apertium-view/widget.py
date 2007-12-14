import gtk.glade

handlers = {}

def gtk_handler(f):
    handlers[f.__name__] = f
    return f

def glade_load_and_connect(fname, **kwargs):
    widget_tree = gtk.glade.XML(fname, **kwargs)
    widget_tree.signal_autoconnect(handlers)
    return widget_tree
