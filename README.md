# Outfit Generator

The Outfit Generator is a simple project designed to aid users in their everyday outfit selection process. The application was developed using Qt and SQLite, offering an interactive and robust experience to the users.

## Features

1. **Database Management**: The application allows users to create and maintain a personal wardrobe database with different categories of clothing items such as tops, bottoms, and shoes.

2. **Image Support**: Users can add images of their own clothes to the database. The images are stored efficiently as BLOBs in SQLite database.

_ -- TO DO --
3. **Outfit Generation**: The application can randomly generate outfits from the images stored in the database. It combines one image from each category (tops, bottoms, and shoes) to form a complete outfit.

4. **User Control**: Users have control over the generated outfit. They can manually change individual items of the outfit and can browse through all available items in each category.
_

## How to Use

1. Click on the "Add" button corresponding to each category to add images of your clothes to the database.
2. After you've added your clothes, click the "Generate Outfit" button to generate a random outfit.
3. If you want to change a part of the outfit, click on the category and browse through all available items.
4. Keep experimenting until you find your perfect outfit for the day!

## Dependencies

- Qt 5.15 or later
- SQLite 3


