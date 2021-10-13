from tkinter import *
from tkinter import Menu
from tkinter import messagebox

window = Tk()

window.title("Rootex")

window.geometry('350x200')

menu = Menu(window)
new_item = Menu(menu, tearoff=0)

#new_item.add_command(label='New')
menu.add_cascade(label='File', menu=new_item)
#new_item.add_command(label='Logs')
menu.add_cascade(label='Logs', menu=new_item)

def clicked():

    messagebox.showinfo('Welcome to Rootex', "Welcome to Rootex! \nClick on 'Logs' to see the system logs")

btn = Button(window,text='Click here', command=clicked)
window.config(menu=menu)
btn.grid(column=0,row=0)

window.mainloop()