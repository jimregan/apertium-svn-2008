"""This module implements support for saving and restoring the
state of GTK widgets.

For every GTK widget class for which dumping/loading support is
implemented, this module contains a function of the same name.
For example, there is a function called 'Window', which implements
dumping/loading support for GTK windows.

Such functions need to return two tuples, where the first element
is the dumping function and the second the loading function. Such
a function must also be annotated with @codec_factory.

The function 'Window' should be a good template.

Note that the dumping/loading functions provided by 'Default' are
used for widgets for which no specific dumping/loading support is
provided.

-----------------------------------------------------------------

Before you say WTF, I must note that I did try to add methods for
this functionality to the GTK classes (as I did in the original
Ruby version). An easy way to do this in Python is
class_obj.__dict__['my_method'] = f

But, this won't work for GTK classes, since their class dictionaries
don't allow this.
"""

codec = {}


def codec_factory(f):
    codec[f.__name__] = f()
    return f


@codec_factory
def Default():
    def dump_state(w):
        return {}

    def load_state(w, p):
        pass

    return (dump_state, load_state)


@codec_factory
def Window():
    """Dump/load the state of GTK windows.
    
    We are interested in the size and position of
    a window.
    """
    
    def dump_state(w):
        return { "x_size": w.get_size()[0],
                 "y_size": w.get_size()[1],
                 "x_pos":  w.get_position()[0], 
                 "y_pos":  w.get_position()[1] }

    def load_state(w, p):
        w.move(int(p["x_pos"]), int(p["y_pos"]))
        w.resize(int(p["x_size"]), int(p["y_size"]))

    return (dump_state, load_state)


@codec_factory
def VPaned():
    """Dump/load the state of a GTK vertical pane container.
    
    We are interested in storing the position of the splitter.
    """

    def dump_state(w):
        return { "position": w.get_position() }

    def load_state(w, p):
        w.set_position(int(p["position"]))

    return (dump_state, load_state)


def get_codec(widget):
    """Given a widget, try to find a dump/load tuple in the
    class_name dictionary. If none is found, return the
    default dumper/loader.
    """
    class_name = widget.__class__.__name__
    
    if class_name in codec:
        return codec[class_name]
    else:
        return codec['Default']


def dump_state(widget):
    return get_codec(widget)[0](widget)

def load_state(widget, state):
    get_codec(widget)[1](widget, state)
