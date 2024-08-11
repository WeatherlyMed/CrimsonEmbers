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

init python:
    import renpy

    # Function to determine character sprite direction and running animation
    def character_sprite():
        if xpos < 0.5:
            return renpy.display.image.Image("EliasDefault/run_0.png") # Replace with actual image path
        elif xpos > 0.5:
            return renpy.display.image.Image("EliasDefault/run_1.png") # Replace with actual image path
        else:
            return renpy.display.image.Image("EliasDefault/idle_0.png") # Replace with actual image path

    # Create a sprite manager
    sprite_manager = SpriteManager()

    # Function to update sprite positions
    def sprite_update():
        if sprite_manager.sprites:
            sprite = sprite_manager.sprites[0]
            sprite.x = xpos * renpy.config.screen_width
            sprite.y = ypos * renpy.config.screen_height

    # Function to handle sprite creation
    def create_character_sprite():
        # Determine the correct sprite based on the character's state
        sprite_image = character_sprite()
        sprite = sprite_manager.create(sprite_image)
        sprite.x = xpos * renpy.config.screen_width
        sprite.y = ypos * renpy.config.screen_height
        return sprite

# Define the screen for character movement
screen move_character():
    # Display the room background
    add "bg room"

    # Display the character sprite, updated dynamically
    add sprite_manager as sprite_manager

    # Movement Buttons
    imagebutton idle "images/arrows/arrow_up_idle.png" hover "images/arrows/arrow_up_hover.png" xpos 600 ypos 500 action [SetVariable("ypos", max(ypos - 0.05, 0)), sprite_update]
    imagebutton idle "images/arrows/arrow_down_idle.png" hover "images/arrows/arrow_down_hover.png" xpos 600 ypos 600 action [SetVariable("ypos", min(ypos + 0.05, 1)), sprite_update]
    imagebutton idle "images/arrows/arrow_left_idle.png" hover "images/arrows/arrow_left_hover.png" xpos 500 ypos 550 action [SetVariable("xpos", max(xpos - 0.05, 0)), sprite_update]
    imagebutton idle "images/arrows/arrow_right_idle.png" hover "images/arrows/arrow_right_hover.png" xpos 700 ypos 550 action [SetVariable("xpos", min(xpos + 0.05, 1)), sprite_update]

# Start the game
label start:
    scene bg room
    python:
        create_character_sprite()
    
    show screen move_character

    # Keep the game running
    while True:
        pause
