from widget import *


def get_wTree(widget):
    try:
        return getattr(widget, "wTree")
    except:
        return get_wTree(widget.get_parent())


def get_widget(self, widget):
    wTree = get_wTree(self)
    return wTree.get_widget(widget)


@gtk_handler
def on_statusbar_motion_notify_event(widget, event):
    if widget.button_down:
        get_widget(widget, "btnCollapsed").set_active(False)
        resizee = get_widget(widget, "scrolledwindow")
        
        rect = resizee.get_allocation()
        _, y_delta = event.get_coords()
        
        y_size = rect.height + int(y_delta) - widget.y_offset
        if y_size > 0:
            resizee.set_size_request(-1, y_size)

            
@gtk_handler
def on_statusbar_button_release_event(widget, event):
    widget.button_down = False


@gtk_handler
def on_statusbar_button_press_event(widget, event):
    widget.button_down = True
    widget.y_offset = int(event.get_coords()[1])


@gtk_handler
def on_btnCollapsed_toggled(widget, data=None):
    resizee = get_widget(widget, "scrolledwindow")

    if not widget.get_active():
        resizee.set_size_request(-1, widget.old_height)

    else:
        widget.old_height = resizee.get_allocation().height
        resizee.set_size_request(-1, 1)
    

def make(txt, child):
    wTree = glade_load_and_connect("TextWidget.glade", root="vbox")

    widget = wTree.get_widget("vbox")
    widget.wTree = wTree

    statusbar = wTree.get_widget("statusbar")
    statusbar.button_down = False
    
    wTree.get_widget("entry").set_text(txt)
    wTree.get_widget("viewport").add(child)

    return widget
    
    
