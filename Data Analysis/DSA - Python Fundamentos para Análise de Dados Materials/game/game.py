#  Python has a lot of GUI frameworks, but Tkinter
#  is the only framework that's built into the Python
#  standard library. Tkinter is the standard GUI library
#  for Python. Python when combined with Tkinter provides
#  a fast and easy way to create GUI applications.
#  Tkinter provides a powerful object-oriented interface
#  to the Tk GUI toolkit.

import tkinter as gui
import random
import time

level = int(input("Choose level? from 1 to 5\nType here: "))
# it's going to define the size of the bar in pixels
length = 500/level

# create the GUI application main window.
main = gui.Tk();
main.title("Game");
# The resizable method on the root window takes two
# boolean parameters to describe whether the window
# is resizable in the X and Y direction. To make it
# completely fixed in size, set both parameters to False:
main.resizable(False, False);
# This subcommand returns or sets platform specific attributes associated with a window.
# -topmost boolean: makes sure the window always stays on top of all other windows
main.wm_attributes("-topmost",1)
# things that are inserted on the window goes here

# canvas is is a rectangular area intended for drawing
# pictures or other complex layouts. You can place graphics, text,
# widgets or frames on a Canvas.
# db Border width in pixels. Default is 2.
# bg Normal background color.
canvas = gui.Canvas(main, width=600, height=500, bd=0, highlightthickness=0)
# This geometry manager organizes widgets in blocks before placing them in the parent widget.
canvas.pack()

# That would make it update the window and allow everything to be on it.
main.update();


count = 0
lost = False

# class to create the ball
class Ball:
    # constructor to initialize the attributes of the clas
    def __init__(self, canvas, bar, color):
        self.canvas = canvas
        self.bar = bar
        # creating ball
        self.id = canvas.create_oval(0, 0, 15, 15, fill=color)
        # Canvas class of Tkinter supports a functions which is used to move objects from one position to another in any canvas or tkinter toplevel.
        # initial place
        self.canvas.move(self.id, 245, 200)
        start = [-3, -2, -1, 1, 2, 3]
        random.shuffle(start)

        self.x = start[0]
        self.y = -3

        self.canvas_height = self.canvas.winfo_height()
        self.canvas_width = self.canvas.winfo_width()

    def draw(self):
        self.canvas.move(self.id, self.x, self.y)

        pos = self.canvas.coords(self.id)

        if pos[1] <= 0:
            self.y = 3

        if pos[3] >= self.canvas_height:
            self.y = -3

        if pos[0] <= 0:
            self.x = 3

        if pos[2] >= self.canvas_width:
            self.x = -3

        self.bar_pos = self.canvas.coords(self.bar.id)

        if pos[2] >= self.bar_pos[0] and pos[0] <= self.bar_pos[2]:
            if pos[3] >= self.bar_pos[1] and pos[3] <= self.bar_pos[3]:
                self.y = -3
                global count
                count += 1
                score()

        if pos[3] <= self.canvas_height:
            self.canvas.after(10, self.draw)
        else:
            game_over()
            global lost
            lost = True


class Bar:
    def __init__(self, canvas, color):
        self.canvas = canvas
        self.id = canvas.create_rectangle(0, 0, length, 10, fill=color)
        self.canvas.move(self.id, 200, 400)

        self.x = 0

        self.canvas_width = self.canvas.winfo_width()

        self.canvas.bind_all("<KeyPress-Left>", self.move_left)
        self.canvas.bind_all("<KeyPress-Right>", self.move_right)

    def draw(self):
        self.canvas.move(self.id, self.x, 0)

        self.pos = self.canvas.coords(self.id)

        if self.pos[0] <= 0:
            self.x = 0

        if self.pos[2] >= self.canvas_width:
            self.x = 0

        global lost

        if lost == False:
            self.canvas.after(10, self.draw)

    def move_left(self, event):
        if self.pos[0] >= 0:
            self.x = -3

    def move_right(self, event):
        if self.pos[2] <= self.canvas_width:
            self.x = 3


def start_game(event):
    global lost, count
    lost = False
    count = 0
    score()
    canvas.itemconfig(game, text=" ")

    time.sleep(1)
    bar.draw()
    ball.draw()


def score():
    canvas.itemconfig(score_now, text="Pontos: " + str(count))


def game_over():
    canvas.itemconfig(game, text="Game over!")


bar = Bar(canvas, "orange")
ball = Ball(canvas, bar, "purple")

score_now = canvas.create_text(430, 20, text="Points: " + str(count), fill="green", font=("Arial", 16))
game = canvas.create_text(400, 300, text=" ", fill="red", font=("Arial", 40))

canvas.bind_all("<Button-1>", start_game)

# There is a method known by the name mainloop()
# is used when your application is ready to run. mainloop()
# is an infinite loop used to run the application, wait for
# an event to occur and process the event as long as the
# window is not closed.
main.mainloop()

# Tkinter provides various controls, such as buttons,
# labels and text boxes used in a GUI application.
# These controls are commonly called widgets.