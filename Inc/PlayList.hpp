
#ifndef PLAYLIST_HPP_DEFINED
#define PLAYLIST_HPP_DEFINED

#include "petit_fs/PetitFs.hpp"

class PlayList
{
public:
    
    PlayList( const char* root_directory_path );
    ~PlayList() noexcept;

    void NextEntry();
    const char* CurrentFileName() const;

private:

    petitfs::FileInfo findFirstFile();
    petitfs::FileInfo findNextFile();

    bool              m_IsExistFile;
    petitfs::Dir      m_Directory;
    petitfs::FileInfo m_CurrentFileInfo;
    petitfs::FileInfo m_PrevFileInfo;
};

#endif  // PLAYLIST_HPP_DEFINED
