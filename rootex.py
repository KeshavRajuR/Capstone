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

    def aboutProject():
        prj = Tk()
        prj.title('About the Project')
        T = Text(prj, height=25, width =100)
        L = Label(prj, text='About the Project')
        L.config(font=('Courier', 30))
        T.config(font=('Roboto', 14))
        About = """ \nProblem Definition: To create a Monitoring System that can detect Kernel Rootkits and block them before causing damage to the system. Rootkits are malware that hide intrusions and maintain privileged access illegally while staying anonymous on the system. Once a system is infected with a rootkit, it is a very hard process to remove it.
        \nIntroduction to the Project: With this project, we hope to monitor the buses for Rootkit entry into the kernel, and alert the user of a possible breach. The model will use signatures to identify the rootkit and stop them before they are successfully installed on the victim machine.
"""
        b2 = Button(prj, text = "Exit", command = prj.destroy) 
        L.pack()
        T.pack()
        b2.pack()
        T.insert(tk.END, About)
        prj.mainloop()

    def aboutTeam():
        prj = Tk()
        prj.title('About the Team')
        T = Text(prj, height=25, width=100)
        #T.tag_configure('Tx', justify=CENTER)
        L = Label(prj, text='About the Team')
        L.config(font=('Courier', 30))
        T.config(font=('Roboto', 14))
        About = """\n This is a dissertation titled 'Rootex: A New Way To Counter Rootkits' is a bonafide work carried out by:              \n Keshav Raju R(PES2201800135)             \n K.A.Vamsi Krishna (PES2201800147)                \n Darshan Shetty (PES2201800027) \n\nThis project was created in partial fulfilment for the completion of the seventh semester Capstone Project Phase-2 (UEI8CS390A) in the program of study - Bachelors in Technology in Computer Science and Engineering under rules and regulations of PES \nUniversity, Bengaluru during the period of June 2021 - November 2021  """

        L.pack()
        T.pack()
        T.insert(tk.END, About)
        #T.tag_add('Tx','1.0', About)
        prj.mainloop()

    def clicked():
        self = Tk()
        self.title('Logs')
        txtarea = Text(self, width=100, height=100)
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
        self.mainloop()

    
    class newWindow(Toplevel):
        def __init__(self, master = None):
            super().__init__(master = root)
            txtarea = Text(self, width=100, height=100)
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
    new_item = Menu(menu, tearoff=0)
    menu.add_cascade(label='About', menu=new_item)
    new_item.add_command(label='About Project', command= aboutProject)
    new_item.add_separator()
    new_item.add_command(label='About Team', command=aboutTeam)
    root.config(menu=menu)


    btn = Button(root,text='Click here to view Logs', height = '5', width = '8')
    fontStyle = tkFont.Font(family="Lucida Grande", size=20)
    labelExample = tk.Label(root, text="20", font=fontStyle)
    btn.pack(expand=True, fill=X, padx=20)
    btn.bind("<Button>", lambda e: newWindow(root))


    root.mainloop()

if __name__ =='__main__':
    main()

