#pragma once

class Texture{

    public :
        void loadTexture(const char* path);

        unsigned int m_textureID = 0;
    
    private :
        int width, height, nrChannels;
        unsigned char *data;
};