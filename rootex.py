from tkinter import *
from tkinter import Tk, ttk
import tkinter as tk
import PIL 
from tkinter import Menu
from tkinter import messagebox
from PIL import ImageTk, Image
import os
from tkinter import filedialog
import tkinter.font as tkFont


def main():
    root = Tk()
    root.title("Rootex")
    global img
    img = ImageTk.PhotoImage(Image.open("rootex.png"))
    panel = Label(root, image = img)
    panel.image = img
    panel.pack(side = "top", fill = "both", expand = "yes")
    root.geometry('900x900')


    def clicked():
        tf = filedialog.askopenfilename(initialdir="events.log", title = "Show Logs", filetypes = (("Text Files", "events.log"),))
        pathh.insert(END, tf)
        tf = open(tf)
        data = tf.read()
        txtarea.insert(END, data)
        tf.close()
    
    class newWindow(Toplevel):
        def __init__(self, master = None):
            super().__init__(master = root)
            txtarea = Text(self, width=40, height=20)
            txtarea.pack(pady=20)        
            pathh = Entry(self)
            pathh.pack(side=LEFT, expand=True, fill=X, padx=20)
            self.title("Logs")
            self.geometry("800x800")
            #label = Label(self, text ="This is a new Window")
            # label.pack()
            tf = filedialog.askopenfilename(initialdir="events.log", title = "Show Logs", filetypes = (("Text Files", "events.log"),))
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
    root.config(menu=menu)


    btn = Button(root,text='Click here to view Logs', height = '5', width = '8')
    fontStyle = tkFont.Font(family="Lucida Grande", size=20)
    labelExample = tk.Label(root, text="20", font=fontStyle)
    btn.pack(expand=True, fill=X, padx=20)
    btn.bind("<Button>", lambda e: newWindow(root))


    root.mainloop()

if __name__ =='__main__':
    main()

