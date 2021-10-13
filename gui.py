#!/usr/bin/python
# -*- coding: utf-8 -*-

from gi.repository import Gtk

class Handler:

    def button_is_clicked(self, button):
        ## The ".run()" method is used to launch the about window.
         ouraboutwindow.run()
        ## This is just a workaround to enable closing the about window.
         ouraboutwindow.hide()

    def enter_button_clicked(self, button):
        ## The ".get_text()" method is used to grab the text from the entry box. The "get_active_text()" method is used to get the selected item from the Combo Box Text widget, here, we merged both texts together".
         print (ourentry.get_text() + ourcomboboxtext.get_active_text())

## Nothing new here.. We just imported the 'ui.glade' file.
builder = Gtk.Builder()
builder.add_from_file("ui.glade")
builder.connect_signals(Handler())

ournewbutton = builder.get_object("button1")

window = builder.get_object("window1")

## Here we imported the Combo Box widget in order to add some change on it.
ourcomboboxtext = builder.get_object("comboboxtext1")

## Here we defined a list called 'default_text' which will contain all the possible items in the Combo Box Text widget.
default_text = [" World ", " Earth ", " All "]

## This is a for loop that adds every single item of the 'default_text' list to the Combo Box Text widget using the '.append_text()' method.
# for x in default_text:
#   ourcomboboxtext.append_text(x)

## The '.set.active(n)' method is used to set the default item in the Combo Box Text widget, while n = the index of that item.
#ourcomboboxtext.set_active(0)
ourentry = builder.get_object("entry1")

## This line doesn't need an explanation :D
#ourentry.set_max_length(15)

## Nor this do.
#ourentry.set_placeholder_text("Enter A Text Here..")

## We just imported the about window here to the 'ouraboutwindow' global variable.
ouraboutwindow = builder.get_object("aboutdialog1")

## Give that developer a cookie !
window.connect("delete-event", Gtk.main_quit)
window.show_all()
Gtk.main
