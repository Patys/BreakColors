#ifndef RESOURCEMENAGER_H
#define RESOURCEMENAGER_H

class ResourceMenager
{
    public:
        ResourceMenager();
        ~ResourceMenager();

    private:
        ResourceMenager( const ResourceMenager& );
        ResourceMenager& operator =( const ResourceMenager& );

    public:
        const sf::Texture&    getTexture( const std::string& filename );
        void deleteTexture( const sf::Texture& texture );
        void deleteTexture( const std::string& filename );
        void addResourceDirectory( const std::string& directory );
        void removeResourceDirectory( const std::string& directory );

    private:
        std::map< std::string, sf::Texture > m_textures;
        std::vector< std::string > m_resource_directories;
};

#endif // RESOURCEMENAGER_H
