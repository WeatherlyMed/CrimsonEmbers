# script.rpy
# Import Ren'Py's functions
define e = Character("Elias")

# Image Declarations
image bg room = "images/scenes/dungeon.png"

# Elias animations based on provided files
image elias_idle = Animation("images/Elias/EliasDefault/idle_0.png", 0.2,
                             "images/Elias/EliasDefault/idle_1.png", 0.2,
                             "images/Elias/EliasDefault/idle_2.png", 0.2,
                             "images/Elias/EliasDefault/idle_3.png", 0.2)

image elias_run_right = Animation("images/Elias/EliasDefault/run_0.png", 0.1,
                                  "images/Elias/EliasDefault/run_1.png", 0.1,
                                  "images/Elias/EliasDefault/run_2.png", 0.1,
                                  "images/Elias/EliasDefault/run_3.png", 0.1)

image elias_run_left = Animation(im.Flip("images/Elias/EliasDefault/run_0.png", horizontal=True), 0.1,
                                 im.Flip("images/Elias/EliasDefault/run_1.png", horizontal=True), 0.1,
                                 im.Flip("images/Elias/EliasDefault/run_2.png", horizontal=True), 0.1,
                                 im.Flip("images/Elias/EliasDefault/run_3.png", horizontal=True), 0.1)

# Set the starting position for the character
default xpos = 0.5
default ypos = 0.5

# Define the screen for character movement
screen move_character():
    # Display the room background
    add "bg room"

    # Display the character sprite, updated dynamically
    add DynamicDisplayable("character_sprite", xpos=xpos, ypos=ypos)

    # Movement Buttons
    imagebutton idle "images/arrows/arrow_up_idle.png" hover "images/arrows/arrow_up_hover.png" xpos 600 ypos 500 action SetVariable("ypos", ypos-0.05)
    imagebutton idle "images/arrows/arrow_down_idle.png" hover "images/arrows/arrow_down_hover.png" xpos 600 ypos 600 action SetVariable("ypos", ypos+0.05)
    imagebutton idle "images/arrows/arrow_left_idle.png" hover "images/arrows/arrow_left_hover.png" xpos 500 ypos 550 action SetVariable("xpos", xpos-0.05)
    imagebutton idle "images/arrows/arrow_right_idle.png" hover "images/arrows/arrow_right_hover.png" xpos 700 ypos 550 action SetVariable("xpos", xpos+0.05)

# Function to determine character sprite direction and running animation
init python:
    def character_sprite(st, at):
        if xpos < 0.5:
            return renpy.display.im.Image("elias_run_left")
        elif xpos > 0.5:
            return renpy.display.im.Image("elias_run_right")
        else:
            return renpy.display.im.Image("elias_idle")

