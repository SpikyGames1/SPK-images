#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

// Function to convert PNG file to custom .spk format
void convert_png_to_spk(const char *png_file, const char *spk_file) {
    // Load the PNG image into an SDL surface
    SDL_Surface *image_surface = IMG_Load(png_file);
    if (!image_surface) {
        printf("Error loading PNG: %s\n", IMG_GetError());
        return;
    }

    // Convert surface to RGB24 format for consistent pixel access
    SDL_Surface *converted_surface = SDL_ConvertSurfaceFormat(image_surface, SDL_PIXELFORMAT_RGB24, 0);
    if (!converted_surface) {
        printf("Error converting surface to RGB24: %s\n", SDL_GetError());
        SDL_FreeSurface(image_surface);
        return;
    }

    // Open the output file for writing in binary mode
    FILE *file = fopen(spk_file, "wb");
    if (!file) {
        printf("Error opening SPK file for writing.\n");
        SDL_FreeSurface(converted_surface);
        SDL_FreeSurface(image_surface);
        return;
    }

    // Write image dimensions (width and height)
    fwrite(&converted_surface->w, sizeof(int), 1, file);
    fwrite(&converted_surface->h, sizeof(int), 1, file);

    // Write pixel data (RGB only)
    Uint8 *pixels = (Uint8 *)converted_surface->pixels;
    for (int y = 0; y < converted_surface->h; y++) {
        for (int x = 0; x < converted_surface->w; x++) {
            // Write RGB pixel data for each pixel
            fwrite(&pixels[(y * converted_surface->pitch) + (x * 3)], sizeof(Uint8), 3, file);
        }
    }

    // Close the file and free surfaces
    fclose(file);
    SDL_FreeSurface(converted_surface);
    SDL_FreeSurface(image_surface);
    printf("PNG to SPK conversion completed: %s\n", spk_file);
}

// Function to display the custom .spk image
void display_spk_image(const char *spk_file) {
    // Open the SPK file for reading in binary mode
    FILE *file = fopen(spk_file, "rb");
    if (!file) {
        printf("Error opening SPK file for reading.\n");
        return;
    }

    // Read image dimensions (width and height)
    int width, height;
    fread(&width, sizeof(int), 1, file);
    fread(&height, sizeof(int), 1, file);

    // Create an SDL window and renderer to display the image
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SPK Image Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create a texture to hold the image data
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, width, height);

    // Allocate memory for pixel data
    Uint8 *pixels = (Uint8 *)malloc(width * height * 3); // 3 bytes per pixel for RGB
    if (!pixels) {
        printf("Error allocating memory for pixel data.\n");
        fclose(file);
        return;
    }

    // Read pixel data from the SPK file
    fread(pixels, sizeof(Uint8), width * height * 3, file);

    // Update the texture with the pixel data
    SDL_UpdateTexture(texture, NULL, pixels, width * 3);

    // Render the texture
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    // Event handling loop
    SDL_Event event;
    while (1) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
    }

    // Cleanup
    free(pixels);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    fclose(file);
}

// Main function to handle command-line arguments and execute the program
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input_png_file> <output_spk_file>\n", argv[0]);
        return 1;
    }

    const char *png_file = argv[1];
    const char *spk_file = argv[2];

    // Convert the PNG file to the custom .spk format
    convert_png_to_spk(png_file, spk_file);

    // Display the converted SPK image
    display_spk_image(spk_file);

    return 0;
}
