#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "../include/ResourceMenager.h"

ResourceMenager::ResourceMenager() : m_textures(), m_resource_directories()
{
}

ResourceMenager::~ResourceMenager()
{
    m_textures.clear();
    m_resource_directories.clear();
}

const sf::Texture& ResourceMenager::getTexture( const std::string& filename )
{
    // Check, whether the image already exists
    for( std::map<std::string, sf::Texture>::const_iterator it = m_textures.begin();
         it != m_textures.end();
         ++it)
    {
        if( filename == it->first )
        {
            //std::cout << "DEBUG_MESSAGE: " << filename << " using existing image.\n";
            return it->second;
        }
    }

    // The image doesen't exists. Create it and save it.
    sf::Texture image;

    // Search project's main directory
    if( image.loadFromFile( filename ) )
    {
        m_textures[filename] = image;
        std::cout << "DEBUG_MESSAGE: " << filename << " loading image.\n";
        return m_textures[filename];
    }

    // If the image has still not been found, search all registered directories
    for( std::vector< std::string >::iterator it = m_resource_directories.begin();
         it != m_resource_directories.end();
         ++it )
    {
        if( image.loadFromFile( (*it) + filename ) )
        {
            m_textures[filename] = image;
            std::cout << "DEBUG_MESSAGE: " << filename << " loading image.\n";
            return m_textures[filename];
        }

    }

    std::cout << "GAME_ERROR: Texture was not found. It is filled with an empty image.\n";
    m_textures[filename] = image;
    return m_textures[filename];
}

void ResourceMenager::deleteTexture( const sf::Texture& image )
{
    for( std::map<std::string, sf::Texture>::const_iterator it = m_textures.begin();
         it != m_textures.end();
         ++it)
    {
        if( &image == &it->second )
        {
            m_textures.erase( it );
                        return;
        }
    }
}

void ResourceMenager::deleteTexture( const std::string& filename )
{
    std::map<std::string, sf::Texture>::const_iterator it = m_textures.find( filename );
    if( it != m_textures.end() )
        m_textures.erase( it );
}

void ResourceMenager::addResourceDirectory( const std::string& directory )
{
    // Check whether the path already exists
    for( std::vector<std::string>::const_iterator it  = m_resource_directories.begin();
         it != m_resource_directories.end();
        ++it )
    {
        // The path exists. So it isn't necessary to add id once more.
        if( directory == (*it) )
            return;
    }

    // insert the directory
    m_resource_directories.push_back( directory );
}

void ResourceMenager::removeResourceDirectory( const std::string& directory )
{
    for( std::vector<std::string>::iterator it  = m_resource_directories.begin();
         it != m_resource_directories.end(); )
    {
        // The path exists. So it isn't necessary to add id once more.
        if( directory == (*it) )
            it = m_resource_directories.erase( it );
        else
            ++it;
    }
}
