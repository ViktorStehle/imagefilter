#include "helpers.h"
#include "math.h"
#include <stdio.h>
#include <cs50.h>

int check255(int storage);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //loop over each array
    for (int i = 0; i < height; i++)
    {
        //loop over each pixel of the array
        for (int j = 0; j < width; j++)
        {
            // get all the colours of this pixxel
            // calculate the average amount of the pixel and round it
            int storage = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.00);
            // store the new value in each colour
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = storage;
        }
    }
    return;
}

// reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //loop over each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            //store the pixel temporary in a storage for rgb
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            //store the end pixel in the beginning of the picture
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            //store the first pixel in the end with storage
            image[i][width - j - 1].rgbtRed = red;
            image[i][width - j - 1].rgbtGreen = green;
            image[i][width - j - 1].rgbtBlue = blue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float count[height][width];
    float copyred[height][width];
    float copygreen[height][width];
    float copyblue[height][width];
    //loop over each pixel of the picture
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // make new grit around the current pixel to blur it
            for (int k = i - 1; k < (i + 2); k++)
            {
                for (int l = j - 1; l < (j + 2); l++)
                {
                    //check the edges
                    if ((k >= 0) && (k < height) && (l >= 0) && (l < width))
                    {
                        // soften the pixels individually and store it in a temporary storage and make a count for division
                        copyred[i][j] += image[k][l].rgbtRed;
                        copygreen[i][j] += image[k][l].rgbtGreen;
                        copyblue[i][j] += image[k][l].rgbtBlue;
                        count[i][j]++;
                    }
                }
            }
        }
    }
    //loop over the picture again and sawp the picture with the copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = round(copyred[i][j] / count[i][j]);
            image[i][j].rgbtGreen = round(copygreen[i][j] / count[i][j]);
            image[i][j].rgbtBlue = round(copyblue[i][j] / count[i][j]);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //storing the multipliers in an array
    double vgx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    double vgy[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    // temporary value storage (lets fool around with structs)
    typedef struct
    {
        float xred;
        float xgreen;
        float xblue;
        float yred;
        float ygreen;
        float yblue;
    }
    gxy;

    // storing this values as 2 dimensional arrays
    gxy xy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            xy[i][j].xred = xy[i][j].xgreen = xy[i][j].xblue = xy[i][j].yred = xy[i][j].ygreen = xy[i][j].yblue = 0;
        }
    }

    //create 2 loops to loop over the picture
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // count for the multiplier array
            int count = 0;
            //create 2loops for the 3x3 grid around the pixel
            for (int k = (i - 1); k < (i + 2) ; k++)
            {
                for (int l = (j - 1); l < (j + 2); l++)
                {
                    // check for the edges of the picture
                    if ((k >= 0) && (k < height) && (l >= 0) && (l < width))
                    {
                        //Gx multiplieing with values
                        xy[i][j].xred += (image[k][l].rgbtRed * (vgx[count]));
                        xy[i][j].xgreen += (image[k][l].rgbtGreen * (vgx[count]));
                        xy[i][j].xblue += (image[k][l].rgbtBlue * (vgx[count]));
                        //Gy multiplieing with values
                        xy[i][j].yred += (image[k][l].rgbtRed * (vgy[count]));
                        xy[i][j].ygreen += (image[k][l].rgbtGreen * (vgy[count]));
                        xy[i][j].yblue += (image[k][l].rgbtBlue * (vgy[count]));
                    }
                    else
                    {
                        xy[i][j].xred += 0;
                        xy[i][j].xgreen += 0;
                        xy[i][j].xblue += 0;
                        xy[i][j].yred += 0;
                        xy[i][j].ygreen += 0;
                        xy[i][j].yblue += 0;
                    }
                    count ++;
                }
            }
        }
    }
    //feed the Gx and Gy values into the Red then Green then Blue: Square root from Gx^2 + Gy^2
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = check255(round(sqrt((xy[i][j].xred * xy[i][j].xred) + (xy[i][j].yred * xy[i][j].yred))));
            image[i][j].rgbtGreen = check255(round(sqrt((xy[i][j].xgreen * xy[i][j].xgreen) + (xy[i][j].ygreen * xy[i][j].ygreen))));
            image[i][j].rgbtBlue = check255(round(sqrt((xy[i][j].xblue * xy[i][j].xblue) + (xy[i][j].yblue * xy[i][j].yblue))));
        }
    }
    return;
}


// Negativ Picture
void negativ(int height,int width,RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = (255 - image[i][j].rgbtRed);
            image[i][j].rgbtGreen = (255- image[i][j].rgbtGreen);
            image[i][j].rgbtBlue = (255 - image[i][j].rgbtBlue);
        }
    }
    return;
}

//Filter for red coloured pictures
void redcolour(int height,int width,RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtGreen = 0;
            image[i][j].rgbtBlue = 0;
        }
    }
}

//Filter for blue coloured pictures
void bluecolour(int height,int width,RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtGreen = 0;
            image[i][j].rgbtRed = 0;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int storagered = check255(round((0.393 * image[i][j].rgbtRed) + (0.769 * image[i][j].rgbtGreen) + (0.189 * image[i][j].rgbtBlue)));
            int storagegreen = check255(round((0.349 * image[i][j].rgbtRed) + (0.686 * image[i][j].rgbtGreen) +
                                              (0.168 * image[i][j].rgbtBlue)));
            int storageblue = check255(round((0.272 * image[i][j].rgbtRed) + (0.534 * image[i][j].rgbtGreen) + (0.131 * image[i][j].rgbtBlue)));
            image[i][j].rgbtBlue = storageblue;
            image[i][j].rgbtGreen = storagegreen;
            image[i][j].rgbtRed = storagered;
        }
    }
    return;
}


int check255(int storage)
{
    if (storage > 255)
    {
        storage = 255;
    }
    if (storage < 0)
    {
        storage = 0;
    }
    return storage;
}