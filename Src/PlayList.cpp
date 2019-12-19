
#include "PlayList.hpp"
#include "UARTOut.hpp"
    
PlayList::PlayList( const char* root_directory_path )
 : m_IsExistFile( false ),
   m_Directory( root_directory_path ),
   m_CurrentFileInfo(),
   m_PrevFileInfo()
{
    m_CurrentFileInfo = findFirstFile();
    if( m_CurrentFileInfo.IsValid() && m_CurrentFileInfo.IsFile() ){
        m_IsExistFile = true;
    }

    exlib::sout << "CurrentFileName : " << m_CurrentFileInfo.Name() << " \n";
}

PlayList::~PlayList()
{}

void PlayList::NextEntry()
{
    if( !m_IsExistFile ){
        return;
    }

    m_PrevFileInfo = m_CurrentFileInfo;
    m_CurrentFileInfo = findNextFile();

    exlib::sout << "CurrentFileName : " << m_CurrentFileInfo.Name() << " \n";
}

const char* PlayList::CurrentFileName() const
{
    if( !m_CurrentFileInfo.IsValid() ){
        return nullptr;
    }

    return m_CurrentFileInfo.Name();
}

petitfs::FileInfo PlayList::findFirstFile()
{
    petitfs::FileInfo fileinfo;

    m_Directory.Reset();
    while( 1 ){
        fileinfo = m_Directory.NextEntry();
        if( !fileinfo.IsValid() || fileinfo.IsFile() ){
            break;
        }
    }

    return fileinfo;
}

petitfs::FileInfo PlayList::findNextFile()
{
    petitfs::FileInfo fileinfo;

    while( !fileinfo.IsFile() ){
        fileinfo = m_Directory.NextEntry();

        if( !fileinfo.IsValid() ){
            m_Directory.Reset();
        }
    }

    return fileinfo;
}