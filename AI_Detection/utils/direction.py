# This module provides a function to determine the direction of an object based on its coordinates and the dimensions of the image.
# The function `get_direction` takes the x and y coordinates of the object, as well as the width and height of the image, and returns a string indicating the direction (North, South, East, West, or Center) based on the position of the object within the image.

def get_direction(x, y, width, height): # Determine the direction based on the position of the object in the image

    if y < height * 0.33: # If the y coordinate is in the top third of the image, return "North"
        return "North"

    elif y > height * 0.66: # If the y coordinate is in the bottom third of the image, return "South"
        return "South"

    elif x < width * 0.33: # If the x coordinate is in the left third of the image,
        return "West"

    elif x > width * 0.66: # If the x coordinate is in the right third of the image,
        return "East"

    return "Center" # If the object is in the center of the image, return "Center"