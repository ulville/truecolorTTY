#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include <math.h>
#include <Imlib2.h>

struct RGBColor
{
    int R, G, B;
};

void print_rgb_pixel(int r, int g, int b)
{
    printf("\x1b]P1%02x%02x%02x", r, g, b);
    printf("\x1b[00;31m█\x1b[00m");
}

struct RGBColor HSVtoRGB(float H, float S, float V)
{
    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;
    if (H >= 0 && H < 60)
    {
        r = C, g = X, b = 0;
    }
    else if (H >= 60 && H < 120)
    {
        r = X, g = C, b = 0;
    }
    else if (H >= 120 && H < 180)
    {
        r = 0, g = C, b = X;
    }
    else if (H >= 180 && H < 240)
    {
        r = 0, g = X, b = C;
    }
    else if (H >= 240 && H < 300)
    {
        r = X, g = 0, b = C;
    }
    else
    {
        r = C, g = 0, b = X;
    }

    struct RGBColor result_rgb = {(r + m) * 255, (g + m) * 255, (b + m) * 255};

    return result_rgb;
}

void draw_hsv_colors()
{
    const float width = 124;
    const float height = 62;
    const float hue_interval = 360 / width;
    const float sat_val_interval = 200 / height;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float h = hue_interval * x;
            float s = sat_val_interval * (y + 1) < 100 ? sat_val_interval * (y + 1) : 100;
            float v = sat_val_interval * (y + 1) > 100 ? 200 - (sat_val_interval * (y + 1)) : 100;

            struct RGBColor rgb = HSVtoRGB(h, s, v);

            print_rgb_pixel(rgb.R, rgb.G, rgb.B);
        }
        printf("\n");
    }
}

void draw_rgb_color_table()
{
    const int colorNum = 15;
    const int interval = 255 / colorNum;

    for (int r = 0; r < colorNum + 1; r++)
    {
        int R = r * interval;
        for (int g = 0; g < colorNum; g++)
        {
            int G = g * interval;
            for (int b = 0; b < colorNum; b++)
            {
                int B = b * interval;
                print_rgb_pixel(R, G, B);
            }
        }
        printf("\n");
    }
}

void draw_image_from_file(const char *path)
{
    struct winsize win;
    ioctl(0, TIOCGWINSZ, &win);
    const int W_Lim = win.ws_col;
    const int H_Lim = win.ws_row - 4;
    const double Ratio = (double)W_Lim / (double)H_Lim;
    Imlib_Image image;
    image = imlib_load_image(path);
    if (image)
    {
        imlib_context_set_image(image);
        int w = imlib_image_get_width();
        int h = imlib_image_get_height();
        double ratio = (double)w / (double)h;
        int new_w = W_Lim;
        int new_h = H_Lim;
        int larger_than_screen = w * 2 > W_Lim && h * 2 > H_Lim;
        int img_col;

        if (larger_than_screen)
        {
            if (ratio * 2.0 < Ratio)
            {
                new_w = new_h * 2.0 * ratio;
            }
            else
            {
                new_h = (int)((double)new_w / (2.0 * ratio));
            }
            img_col = new_w;
            Imlib_Image scaled_image = imlib_create_cropped_scaled_image(0, 0, w, h, new_w, new_h);
            imlib_context_set_image(scaled_image);
        }
        else
        {
            new_h = h;
            new_w = w;
            img_col = w * 2;
        }

        unsigned char *pixels = (unsigned char *)imlib_image_get_data_for_reading_only();

        for (size_t y = 0; y < new_h; y++)
        {
            // center the image
            (win.ws_col - img_col > 1) ? printf("%*c", (win.ws_col - img_col) / 2, ' ') : printf("");

            for (size_t x = 0; x < new_w; x++)
            {
                unsigned int B = (unsigned int)*(pixels + 4 * (x + y * new_w));
                unsigned int G = (unsigned int)*(pixels + 4 * (x + y * new_w) + 1);
                unsigned int R = (unsigned int)*(pixels + 4 * (x + y * new_w) + 2);
                unsigned int A = (unsigned int)*(pixels + 4 * (x + y * new_w) + 3);
                print_rgb_pixel((R * A) / 255, (G * A) / 255, (B * A) / 255);
                larger_than_screen ?: print_rgb_pixel((R * A) / 255, (G * A) / 255, (B * A) / 255);
            }
            printf("\n");
        }

        imlib_free_image();
    }
}

int main(int argc, char const *argv[])
{
    const char *terminal = getenv("TERM");
    if (terminal == NULL)
    {
        printf("getenv(\"TERM\") returned NULL");
        return 1;
    }

    if (strcmp(terminal, "linux") == 0)
    {
        if (argc > 1 && strcmp(argv[1], "hsv") == 0)
        {
            draw_hsv_colors();
        }
        else if (argc > 1 && strcmp(argv[1], "rgb") == 0)
        {
            draw_rgb_color_table();
        }
        else if (argc > 2 && argv[1][0] == '-' && argv[1][1] == 'f')
        {
            const char *path = argv[2];
            draw_image_from_file(path);
        }

        printf("\x1b]P1aa0000\n");
    }
    else
    {
        printf("Terminal is \"%s\"\nThis only works in \"linux\" terminal\n", terminal);
        return 2;
    }

    return 0;
}
