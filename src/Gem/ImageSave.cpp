////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "ImageIO.h"
#include "Gem/RTE.h"
#include "Gem/Files.h"

#include "plugins/imagesaver.h"

namespace gem {
  static std::map<std::string, std::string>*s_extension2mime=NULL;

  static void build_extension2mime(void) {
    if(NULL==s_extension2mime)
      s_extension2mime = new  std::map<std::string, std::string>;
    std::map<std::string, std::string>&e2m=*s_extension2mime;

    // avoid the use of '*/x-*' mimetypes as the "x-" is actually private
    // if a file-extension matches multiple mimetypes, chose the most obvious
    /* this is generated with something like:
       $  cat mimetypes.txt | while read mime exts; do for ext in $exts; do echo "e2m[\"${ext}\"]=\"${mime}\";"; done; done | sort 
    */

    e2m["art"]="image/x-jg";
    e2m["avs"]="image/avs";
    e2m["bie"]="image/bie";
    e2m["bm"]="image/bmp";
    e2m["bmp"]="image/bmp";
    //e2m["bmp"]="image/x-windows-bmp";
    //e2m["bmp"]="image/x-ms-bmp";
    e2m["cmx"]="image/x-cmx";
    e2m["cmyk"]="image/cmyk";
    e2m["cod"]="image/cis-cod";
    e2m["dwg"]="image/vnd.dwg";
    //e2m["dwg"]="image/x-dwg";
    e2m["dxf"]="image/vnd.dwg";
    //e2m["dxf"]="image/x-dwg";
    e2m["eps"]="image/x-eps";
    e2m["epsf"]="image/x-eps";
    e2m["epsi"]="image/x-eps";
    e2m["eps.bz2"]="image/x-bzeps";
    e2m["eps.gz"]="image/x-gzeps";
    e2m["fif"]="image/fif";
    e2m["fits"]="image/fits";
    e2m["flo"]="image/florian";
    e2m["fpx"]="image/vnd.fpx";
    //e2m["fpx"]="image/vnd.net-fpx";
    e2m["g3"]="image/g3fax";
    e2m["gif"]="image/gif";
    e2m["icb"]="image/targa";
    e2m["ico"]="image/x-icon";
    e2m["ief"]="image/ief";
    e2m["iefs"]="image/ief";
    e2m["jfif"]="image/jpeg";
    //e2m["jfif"]="image/pjpeg";
    e2m["jfif-tbnl"]="image/jpeg";
    e2m["jng"]="image/x-jng";
    e2m["jpeg"]="image/jpeg";
    //e2m["jpeg"]="image/pjpeg";
    e2m["jpe"]="image/jpeg";
    //e2m["jpe"]="image/pjpeg";
    e2m["jpg"]="image/jpeg";
    //e2m["jpg"]="image/pjpeg";
    e2m["jps"]="image/x-jps";
    e2m["jut"]="image/jutvision";
    e2m["mcf"]="image/vasa";
    e2m["nap"]="image/naplps";
    e2m["naplps"]="image/naplps";
    e2m["niff"]="image/x-niff";
    e2m["nif"]="image/x-niff";
    e2m["pbm"]="image/x-portable-bitmap";
    e2m["pct"]="image/x-pict";
    e2m["pcx"]="image/pcx";
    //e2m["pcx"]="image/x-pcx";
    e2m["pgm"]="image/x-portable-graymap";
    //e2m["pgm"]="image/x-portable-greymap";
    e2m["pic"]="image/pict";
    e2m["pict"]="image/pict";
    e2m["pm"]="image/x-xpixmap";
    e2m["png"]="image/png";
    //e2m["png"]="image/x-png";
    e2m["pnm"]="image/x-portable-anymap";
    e2m["ppm"]="image/x-portable-pixmap";
    e2m["ps"]="image/ps";
    e2m["qif"]="image/x-quicktime";
    e2m["qtif"]="image/x-quicktime";
    e2m["qti"]="image/x-quicktime";
    e2m["ras"]="image/cmu-raster";
    //e2m["ras"]="image/x-cmu-raster";
    e2m["rast"]="image/cmu-raster";
    e2m["rf"]="image/vnd.rn-realflash";
    e2m["rle"]="image/rle";
    e2m["rgb"]="image/x-rgb";
    e2m["rp"]="image/vnd.rn-realpix";
    e2m["sgi"]="image/sgi";
    e2m["sun"]="image/sun-raster";
    e2m["svf"]="image/vnd.dwg";
    //e2m["svf"]="image/x-dwg";
    e2m["svg"]="image/svg";
    //e2m["svg"]="image/svg+xml";
    e2m["tga"]="image/targa";
    //e2m["tga"]="image/tga";
    e2m["tiff"]="image/tiff";
    //e2m["tiff"]="image/x-tiff";
    e2m["tif"]="image/tiff";
    //e2m["tif"]="image/x-tiff";
    e2m["tpic"]="image/targa";
    e2m["turbot"]="image/florian";
    e2m["vda"]="image/targa";
    e2m["vst"]="image/targa";
    e2m["wbmp"]="image/vnd.wap.wbmp";
    e2m["xbm"]="image/xbm";
    //e2m["xbm"]="image/x-xbitmap";
    //e2m["xbm"]="image/x-xbm";
    e2m["xif"]="image/vnd.xiff";
    e2m["xpm"]="image/xpm";
    //e2m["xpm"]="image/x-xpm";
    //e2m["xpm"]="image/x-xpixmap";
    //e2m["xpm"]="image/x-xpixmap";
    e2m["x-png"]="image/png";
    e2m["xwd"]="image/x-xwd";
    //e2m["xwd"]="image/x-xwindowdump";
  }

  static const std::string imgName2Mime(const std::string filename) {
    std::string empty;
    if(NULL==s_extension2mime) {
      build_extension2mime();
    }
    if(s_extension2mime) {
      std::map<std::string, std::string>::iterator it=s_extension2mime->find(gem::files::getExtension(filename, true));
      if(s_extension2mime->end() != it) {
        return it->second;
      }
    }
    return empty;
  }

  class PixImageSaver : public gem::plugins::imagesaver {
  private:
    static PixImageSaver*s_instance;
    std::vector<gem::plugins::imagesaver*>m_savers;
    std::vector<std::string>m_ids;

    PixImageSaver(void) {
      gem::PluginFactory<gem::plugins::imagesaver>::loadPlugins("image");
      std::vector<std::string>available_ids=gem::PluginFactory<gem::plugins::imagesaver>::getIDs();
      if(available_ids.size()>0) {
        startpost("Image saving support:");
        unsigned int i;
        for(i=0; i<available_ids.size(); i++) {
          startpost(" %s", available_ids[i].c_str());
        }
        endpost();
      }

      addSaver(available_ids, "QT");
      addSaver(available_ids, "magick");
      addSaver(available_ids);
    }
    bool addSaver( std::vector<std::string>available, std::string ID=std::string(""))
    {
      int count=0;

      std::vector<std::string>id;
      if(!ID.empty()) {
        // if requested 'cid' is in 'available' add it to the list of 'id's
        if(std::find(available.begin(), available.end(), ID)!=available.end()) {
          id.push_back(ID);
        } else {
          // request for an unavailable ID
          verbose(2, "backend '%s' unavailable", ID.c_str());
          return false;
        }
      } else {
        // no 'ID' given: add all available IDs
        id=available;
      }

      unsigned int i=0;
      for(i=0; i<id.size(); i++) {
        std::string key=id[i];
        verbose(2, "trying to add '%s' as backend", key.c_str());
        if(std::find(m_ids.begin(), m_ids.end(), key)==m_ids.end()) {
          // not yet added, do so now!
          gem::plugins::imagesaver*saver=
            gem::PluginFactory<gem::plugins::imagesaver>::getInstance(key); 
          if(NULL==saver)break;
          m_ids.push_back(key);
          m_savers.push_back(saver);
          count++;
          verbose(2, "added backend#%d '%s' @ 0x%x", m_savers.size()-1, key.c_str(), saver);
        }
      }
      return (count>0);
    }

  public:
    virtual ~PixImageSaver(void) {
      unsigned int i;
      for(i=0; i<m_savers.size(); i++) {
        delete m_savers[i];
        m_savers[i]=NULL;
      }
    }

    virtual bool save(const imageStruct&img, const std::string&filename, const std::string&mimetype, const gem::Properties&props) {
      std::multimap<float, int>priorities;
      std::multimap<float,int>::reverse_iterator rit;
      unsigned int i;

      for(i=0; i<m_savers.size(); i++) {
        float prio=m_savers[i]->estimateSave(img, filename, mimetype, props);
        priorities.insert( std::multimap<float, int>::value_type(prio, i));
      }

      for(rit=priorities.rbegin(); rit != priorities.rend(); rit++) {
        float prio=rit->first;
        int index=rit->second;
        verbose(2, "trying saver[%d]=%s / %f", index, m_ids[index].c_str(), prio);
        if(m_savers[index]->save(img, filename, mimetype, props))
          return true;
      }

      return false;
    }

    static PixImageSaver*getInstance(void) {
      if(NULL==s_instance) {
        s_instance=new PixImageSaver();
      }
      return s_instance;
    }

  }; };

gem::PixImageSaver*gem::PixImageSaver::s_instance=NULL;

/***************************************************************************
 *
 * mem2image - Save an image to a file
 *
 ***************************************************************************/
GEM_EXTERN int mem2image(imageStruct* image, const char *filename, const int type)
{
  gem::PixImageSaver*piximagesaver=gem::PixImageSaver::getInstance();
  if(piximagesaver) {
    std::string fname=filename;
    std::string mimetype=gem::imgName2Mime(fname);
    gem::Properties props;
    if(type>0) {
      props.set("quality", (float)type);
    }
    if(mimetype=="")
      mimetype=(type>0)?"image/jpeg":"image/tiff";

    if(piximagesaver->save(*image, filename, mimetype, props)) {
      return (1);
    }
  }
  error("GEM: Unable to save image to '%s'", filename);
  return (0);
}