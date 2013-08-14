#include "TextDriver.h"

using namespace std;

TextDriver::TextDriver(const string& font_name, const string& font_sprite_dir)
{
//{{{
    default_ext = ".png";
    FontDesc new_font;
    new_font.font_name = font_name;
    cout<<"Font sprite dir: " << font_sprite_dir<<endl;
    loadLowerCase(new_font, font_sprite_dir, default_ext);
//}}}
}

TextDriver::TextDriver(const std::string& font_name,
                        const std::string& font_sprite_sheet,
                        int element_width, 
                        int element_height,
                        int rows, int columns)
{
//{{{
    FontDesc_sheet new_font;
    char working_char = '!';
    new_font.font_name = font_name;
    cout<<"Font sprite sheet: " << font_sprite_sheet<<endl;

    SDL_Rect tmp_rect;
        tmp_rect.x = 0;
        tmp_rect.y = 0;
        tmp_rect.w = element_width;
        tmp_rect.h = element_height;

    for(int row = 0; row < rows ; row++)
    {
        tmp_rect.y = row * element_height;

        for(int column = 0; column < columns; column++)
        {
            tmp_rect.x = column * element_width;
            new_font.character_set[working_char] = tmp_rect;
            cout<<"-+-+Parsing Character : " << working_char <<endl;
            cout<<"\t--> (x , y) = ("<<tmp_rect.x<<","<<tmp_rect.y<<")"<<endl;
            cout<<"\t--> (w , h) = ("<<tmp_rect.w<<","<<tmp_rect.h<<")"<<endl;
            working_char++;
        }
    }

    //load the sheet
    new_font.font_sheet = new Sprite(font_sprite_sheet);

    font_sheets.push_back(new_font);
//}}}
}


TextDriver::~TextDriver()
{

}

void TextDriver::loadLowerCase(FontDesc &new_font, 
                                const string& font_sprite_dir,
                                const string& extension)
{
//{{{
    string char_img = "a" + extension;

    int temp_width = 0, temp_height = 0;

    new_font.max_width = 0;
    new_font.max_height = 0;
    new_font.char_spacing = 0;
    new_font.line_spacing = 0;

    for(int i = 0; i < 25 ; i++)
    {
        cout<<"Loading character img: "<<font_sprite_dir + char_img<<endl;

        new_font.character_set.insert(std::pair<char, Sprite*>
                (char_img[0], new Sprite(font_sprite_dir + char_img)));

        new_font.character_set[char_img[0]]->getDimensions(temp_width, temp_height);

        cout<<"\t-> Dimensions: "<<temp_width<<" x " << temp_height << endl;

        if(temp_width > new_font.max_width)
            new_font.max_width = temp_width;

        if(temp_height > new_font.max_height)
            new_font.max_height = temp_height;

        char_img[0]++;
    }

    cout<<"MAX WIDTH: " << new_font.max_width << endl;
    cout<<"MAX HEIGHT: " << new_font.max_height << endl;

    new_font.char_spacing = (int)((float)new_font.max_width/2.0);
    new_font.line_spacing = (int)((float)new_font.max_height/2.0);

    cout<<"SPACING: " << new_font.char_spacing << " x " << new_font.line_spacing << endl;
    fonts.push_back(new_font);
//}}}
}

void TextDriver::loadUpperCase(FontDesc& new_font, 
                                const string& font_sprite_dir,
                                const string& extension)
{

}

Surface_Container TextDriver::createTextBox(const std::string& font_name,
                                            const std::string& box_name,
                                            const std::string& text)
{
//{{{
    int box_width = 300, box_height = 300;          //dimensions of the surface needed

    Surface_Container text_box;
    text_box.name = box_name;

    text_box.surface = SDL_DisplayFormatAlpha(Common::CreateSurface(box_width, box_height));
    text_box.surface = Common::CreateSurface(box_width, box_height);

    //SDL_FillRect(text_box.surface, NULL, 0x2f00223f);


    SDL_Rect rect;
    rect.x = 100;
    rect.y = 125;
    rect.w = 25;
    rect.h = 25;
    //SDL_SetAlpha(text_box.surface, SDL_SRCALPHA, 0xff);
    //SDL_SetAlpha(img, 0 , 0xff);
    //SDL_SetAlpha(text_box.surface, 0, 0xff);
    Common::surfaceToSurface(font_sheets.back().font_sheet->getSprite(),
                                rect,
                                text_box.surface, 10,10);



    //rotozoomSurface(text_box.surface, 0.0, 1.0, 0.0);
    //SDL_SetAlpha(text_box.surface,0 , 0);

    //text_box.surface = rotozoomSurface(text_box.surface, 0.0, 1.0, 0);
//    for( int i = 0; i < text.length(); i++)
//    {
//        //check that character is in font
//        if(this->font_sheets.back().character_set.count(text[i]) > 0)
//        {
//            cout<<"PLACING CAHRACTER " << text[i] <<endl;
//            Common::surfaceToSurface(font_sheets.back().font_sheet->getSprite(),
//                                        font_sheets.back().character_set[text.at(i)],
//                                        text_box.surface,
//                                         i * 15, 45);
//        }
//    }

    //SDL_FreeSurface(text_box.surface);
    //text_box.surface = font_sheets.back().font_sheet->getSprite();

    //text_box.surface = font_sheets.back().font_sheet->getSprite();
    return text_box;
//}}}
}



Surface_Container TextDriver::createTextBox(const std::string& box_name, const std::string& text)
{
//{{{
    int box_width = 100, box_height = 100;          //dimensions of the surface needed

    Surface_Container text_box;
    text_box.name = box_name;


    text_box.surface = Common::CreateSurface(box_width, box_height);

    for( int i = 0; i < text.length(); i++)
    {
        //check that character is in font
        if(this->fonts.back().character_set.count(text[i]) > 0)
        {
            cout<<"PLACING CAHRACTER " << text[i] <<endl;
            Common::surfaceToSurface( fonts.back().character_set[text.at(i)]->getBaseSprite(),
                                        text_box.surface, i * 15, 45);
        }
    }

    return text_box;
//}}}
}


void TextDriver::determineTextBoxDimensions(const std::string& text, int &width, int &height)
{
    width = 0;
    height = 0;

    int char_spacing = fonts.back().char_spacing;
    int line_spacing = fonts.back().line_spacing;

    for(int i = 0; i < text.length(); i++)
    {
        if ( text.at(i) != '\n' )
        {
        }
    }
}

void TextDriver::determineFontAttributes(FontDesc font,
                                            int &width, 
                                            int &height,
                                            int &char_spaceing,
                                            int &line_spacing)
{

}


