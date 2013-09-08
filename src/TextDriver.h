#ifndef TEXT_DRIVER_H
#define TEXT_DRIVER_H
#include <vector>
#include <string>
#include <map>
#include <SDL2/SDL_image.h>
#include "Common.h"
#include "Sprite.h"


struct FontDesc
{
    std::string font_name;
    int max_width, max_height;
    int char_spacing, line_spacing;
    std::map<char, Sprite*> character_set;
};

struct FontDesc_sheet
{
    std::string font_name;
    int max_width, max_height;
    int char_spacing, line_spacing;
    Sprite *font_sheet;
    std::map<char, SDL_Rect> character_set;
};

class TextDriver
{
    public:
        //individual sprite file constructor
        TextDriver(const std::string& font_name,const std::string& font_sprite_dir);

        //sprite sheet constructor
        TextDriver(const std::string& font_name,
                    const std::string& font_sprite_sheet,
                    int element_width, 
                    int element_height,
                    int rows, int columns);

        TextDriver(const std::string& font_name, 
                    const std::string& font_sprite_dir,
                    vector<char> char_set);

        ~TextDriver();

        //automatically looks for lower case englich characters in font_sprite_dir
        //ex: "a.png", "b.png", etc.
        void loadLowerCase(FontDesc &new_font, 
                            const std::string& font_sprite_dir,
                            const std::string& extension);

        //automatically looks for upper case englich characters in font_sprite_dir
        //ex: "A.png", "B.png", etc.
        void loadUpperCase(FontDesc &new_font,
                            const std::string& font_sprite_dir,
                            const std::string& extension);

        Surface_Container createTextBox(const std::string& box_name, const std::string& text);
        Surface_Container createTextBox(const std::string& font_name,
                                        const std::string& box_name, 
                                        const std::string& text);

        void determineTextBoxDimensions(const std::string& text, int &width, int &height);

        //determine attributes of a loaded font in order to display it
        void determineFontAttributes(FontDesc font,
                                        int &width, 
                                        int &height,
                                        int &char_spaceing,
                                        int &line_spacing);

        std::string default_ext;
        std::vector<FontDesc> fonts;
        std::vector<FontDesc_sheet> font_sheets;
        std::map<std::string, Surface_Container*> text_boxes;

    private:
};
#endif

