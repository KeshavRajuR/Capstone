from tkinter import *
from tkinter import Tk, ttk
import tkinter as tk
import PIL 
from tkinter import Menu
from tkinter import messagebox
from PIL import ImageTk, Image
import os
from tkinter import filedialog






def main():
    root = Tk()
    root.title("Rootex")
    global img
    img = ImageTk.PhotoImage(Image.open("rootex.png"))
    panel = Label(root, image = img)
    panel.image = img
    panel.pack()
    root.geometry('900x900')


    def clicked():
        tf = filedialog.askopenfilename(initialdir="events.log", title = "Show Logs", filetypes = (("Text Files", "*.log"),))
        pathh.insert(END, tf)
        tf = open(tf)
        data = tf.read()
        txtarea.insert(END, data)
        tf.close()
    
    menu = Menu(root)
    new_item = Menu(menu, tearoff=0)
    menu.add_cascade(label ='File', menu = new_item)
    new_item.add_command(label ='Open Logs', command = clicked)
    new_item.add_separator()
    new_item.add_command(label ='Exit', command = root.destroy)
    

    txtarea = Text(root, width=40, height=20)
    txtarea.pack(pady=20)        
    pathh = Entry(root)
    pathh.pack(side=LEFT, expand=True, fill=X, padx=20)


    btn = Button(root,text='Click here to view Logs', command=clicked)
    #btn.grid(column=0,row=0)
    btn.pack(side=RIGHT, expand=True, fill=X, padx=20)


    root.config(menu=menu)

    
    root.mainloop()

if __name__ =='__main__':
    main()

