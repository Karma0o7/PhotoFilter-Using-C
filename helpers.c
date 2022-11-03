#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int Red = image[i][j].rgbtRed;
            int Green = image[i][j].rgbtGreen;
            int Blue = image[i][j].rgbtBlue;
            int gray = (int)round((Red + Green + Blue) / 3.0);
            image[i][j].rgbtRed = gray;
            image[i][j].rgbtGreen = gray;
            image[i][j].rgbtBlue = gray;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            // Temp Colour holding vairables for swaping
            int tempRed = image[i][j].rgbtRed;
            int tempGreen = image[i][j].rgbtGreen;
            int tempBlue = image[i][j].rgbtBlue;

            // Swaping right-side to the left-side
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;

            // Giving right-side the colours of left-side.
            image[i][width - j - 1].rgbtRed = tempRed;
            image[i][width - j - 1].rgbtGreen = tempGreen;
            image[i][width - j - 1].rgbtBlue = tempBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Initialize copy of image
    RGBTRIPLE temp[height][width];

    // Make a copy of image to preserve original values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Variable that counts how many numbers added to arrive at the sum
            int count = 0;

            // Sum variables for each colour
            double sumRed = 0;
            double sumGreen = 0;
            double sumBlue = 0;

            for (int h = i - 1; h <= i + 1; h++)
            {
                for (int w = j - 1; w <= j + 1; w++)
                {
                    // Only adds pixels that are within the image boundaries
                    if (h >= 0 && w >= 0 && h < height && w < width)
                    {
                        sumRed += temp[h][w].rgbtRed;
                        sumGreen += temp[h][w].rgbtGreen;
                        sumBlue += temp[h][w].rgbtBlue;
                        count++;
                    }
                }
            }
            // Use the averages from the surrounding pixels and set the new colour values for the iterated pixel
            image[i][j].rgbtRed = round(sumRed / count);
            image[i][j].rgbtGreen = round(sumGreen / count);
            image[i][j].rgbtBlue = round(sumBlue / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Temp copy of Image.
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    // Soble operator Matrix for Gx and Gy
    int kernel_Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int kernel_Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize values for weighted sums in the x direction
            double gx_red = 0;
            double gx_green = 0;
            double gx_blue = 0;
            // Initialize values for weighted sums in the y direction
            double gy_red = 0;
            double gy_green = 0;
            double gy_blue = 0;

            // Counter for row.
            int row = 0;
            // Loop to check the surrounding pixels within 1 row
            for (int h = i - 1; h <= i + 1; h++)
            {
                // Counter for column
                int column = 0;
                // Loop to check the surrounding pixels within 1 column
                for (int w = j - 1; w <= j + 1; w++)
                {
                    // Loop to check the surrounding pixels within 1 column
                    if (h >= 0 && w >= 0 && h < height && w < width)
                    {
                        // Calculate Gx
                        gx_red += (kernel_Gx[row][column] * temp[h][w].rgbtRed);
                        gx_green += (kernel_Gx[row][column] * temp[h][w].rgbtGreen);
                        gx_blue += (kernel_Gx[row][column] * temp[h][w].rgbtBlue);
                        // Calculate Gy
                        gy_red += (kernel_Gy[row][column] * temp[h][w].rgbtRed);
                        gy_green += (kernel_Gy[row][column] * temp[h][w].rgbtGreen);
                        gy_blue += (kernel_Gy[row][column] * temp[h][w].rgbtBlue);
                    }
                    column++;
                }
                row++;
            }
            // Combine Gx and Gy
            int sobel_red = round(sqrt(pow(gx_red, 2) + pow(gy_red, 2)));
            int sobel_green = round(sqrt(pow(gx_green, 2) + pow(gy_green, 2)));
            int sobel_blue = round(sqrt(pow(gx_blue, 2) + pow(gy_blue, 2)));

            // Set the new colour values for the iterated pixel and cap at 255 if necessary
            image[i][j].rgbtRed = (sobel_red > 255) ? 255 : sobel_red;
            image[i][j].rgbtGreen = (sobel_green > 255) ? 255 : sobel_green;
            image[i][j].rgbtBlue = (sobel_blue > 255) ? 255 : sobel_blue;
        }
    }
    return;
}
