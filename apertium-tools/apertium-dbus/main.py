#!/usr/bin/env python

usage = """Usage:
python example-service.py &
"""

import gobject

import dbus
import dbus.service as service
import dbus.mainloop.glib


class ApertiumInfo(service.Object):
	interface = "org.apertium.AperiumInfo"

	@service.method(ApertiumInfo.interface, in_signature='', out_signature='s')
	def directory(self):
		return self._directory

	def __init__(self, session_bus):
		service.Object.__init__(self, '/')
		self.name = service.BusName("org.apertium.info", session_bus)


class Translator(service.Object):
	interface = "org.apertium.Translator"

	@service.method(Translator.interface, in_signature='s', out_signature='s')
	def translate(self, text):
		return text

	def __init__(self, session_bus, pair):
		service.Object.__init__(self, '/' + pair)
		self.name = service.BusName("org.apertium.translate", session_bus)


if __name__ == '__main__':
	dbus.mainloop.glib.DBusGMainLoop(set_as_default = True)

	session_bus = dbus.SessionBus()
	// Create translation objects

	gobject.MainLoop().run()

