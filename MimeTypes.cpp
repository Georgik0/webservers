# include "MimeTypes.hpp"

MimeTypes::MimeTypes() {
  mimes_types[".html"] = "text/html";
  mimes_types[".htm"] = "text/html";
  mimes_types[".shtml"] = "text/html";
  mimes_types[".css"] = "text/css";
  mimes_types[".xml"] = "text/xml";
  mimes_types[".gif"] = "image/gif";
  mimes_types[".jpeg"] = "image/jpeg";
  mimes_types[".jpg"] = "image/jpeg";
  mimes_types[".js"] = "application/javascript";
  mimes_types[".atom"] = "application/atom+xml";
  mimes_types[".rss"] = "application/rss+xml";

  mimes_types[".mml"] = "text/mathml";
  mimes_types[".txt"] = "text/plain";
  mimes_types[".jad"] = "text/vnd.sun.j2me.app-descriptor";
  mimes_types[".wml"] = "text/vnd.wap.wml";
  mimes_types[".htc"] = "text/x-component";

  mimes_types[".png"] = "image/png";
  mimes_types[".svg"] = "image/svg+xml";
  mimes_types[".svgz"] = "image/svg+xml";
  mimes_types[".tif"] = "image/tiff";
  mimes_types[".tiff"] = "image/tiff";
  mimes_types[".wbmp"] = "image/vnd.wap.wbmp";
  mimes_types[".webp"] = "image/webp";
  mimes_types[".ico"] = "image/x-icon";
  mimes_types[".jng"] = "image/x-jng";
  mimes_types[".bmp"] = "image/x-ms-bmp";

  mimes_types[".woff"] = "font/woff";
  mimes_types[".woff2"] = "font/woff2";

  mimes_types[".jar"] = "application/java-archive";
  mimes_types[".war"] = "application/java-archive";
  mimes_types[".ear"] = "application/java-archive";
  mimes_types[".json"] = "application/json";
  mimes_types[".hqx"] = "application/mac-binhex40";
  mimes_types[".doc"] = "application/msword";
  mimes_types[".pdf"] = "application/pdf";
  mimes_types[".ps"] = "application/postscript";
  mimes_types[".eps"] = "application/postscript";
  mimes_types[".ai"] = "application/postscript";
  mimes_types[".rtf"] = "application/rtf";
  mimes_types[".m3u8"] = "application/vnd.apple.mpegurl";
  mimes_types[".kml"] = "application/vnd.google-earth.kml+xml";
  mimes_types[".kmz"] = "application/vnd.google-earth.kmz";
  mimes_types[".xls"] = "application/vnd.ms-excel";
  mimes_types[".eot"] = "application/vnd.ms-fontobject";
  mimes_types[".ppt"] = "application/vnd.ms-powerpoint";
  mimes_types[".odg"] = "application/vnd.oasis.opendocument.graphics";
  mimes_types[".odp"] = "application/vnd.oasis.opendocument.presentation";
  mimes_types[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
  mimes_types[".odt"] = "application/vnd.oasis.opendocument.text";

  mimes_types[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
  mimes_types[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
  mimes_types[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";

  mimes_types[".wmlc"] = "application/vnd.wap.wmlc";
  mimes_types[".7z"] = "application/x-7z-compressed";
  mimes_types[".cco"] = "application/x-cocoa";
  mimes_types[".jardiff"] = "application/x-java-archive-diff";
  mimes_types[".jnlp"] = "application/x-java-jnlp-file";
  mimes_types[".run"] = "application/x-makeself";
  mimes_types[".pl"] = "application/x-perl";
  mimes_types[".pm"] = "application/x-perl";
  mimes_types[".prc"] = "application/x-pilot";
  mimes_types[".pdb"] = "application/x-pilot";
  mimes_types[".rar"] = "application/x-rar-compressed";
  mimes_types[".rpm"] = "application/x-redhat-package-manager";
  mimes_types[".sea"] = "application/x-sea";
  mimes_types[".swf"] = "application/x-shockwave-flash";
  mimes_types[".sit"] = "application/x-stuffit";
  mimes_types[".tcl"] = "application/x-tcl";
  mimes_types[".tk"] = "application/x-tcl";
  mimes_types[".der"] = "application/x-x509-ca-cert";
  mimes_types[".pem"] = "application/x-x509-ca-cert";
  mimes_types[".crt"] = "application/x-x509-ca-cert";
  mimes_types[".xpi"] = "application/x-xpinstall";
  mimes_types[".xhtml"] = "application/xhtml+xml";
  mimes_types[".xspf"] = "application/xspf+xml";
  mimes_types[".zip"] = "application/zip";

  mimes_types[".bin"] = "application/octet-stream";
  mimes_types[".exe"] = "application/octet-stream";
  mimes_types[".dll"] = "application/octet-stream";
  mimes_types[".deb"] = "application/octet-stream";
  mimes_types[".dmg"] = "application/octet-stream";
  mimes_types[".iso"] = "application/octet-stream";
  mimes_types[".img"] = "application/octet-stream";
  mimes_types[".msi"] = "application/octet-stream";
  mimes_types[".msp"] = "application/octet-stream";
  mimes_types[".msm"] = "application/octet-stream";

  mimes_types[".mid"] = "audio/midi";
  mimes_types[".midi"] = "audio/midi";
  mimes_types[".kar"] = "audio/midi";
  mimes_types[".mp3"] = "audio/mpeg";
  mimes_types[".ogg"] = "audio/ogg";
  mimes_types[".m4a"] = "audio/x-m4a";
  mimes_types[".ra"] = "audio/x-realaudio";

  mimes_types[".3gpp"] = "video/3gpp";
  mimes_types[".3gp"] = "video/3gpp";
  mimes_types[".ts"] = "video/mp2t";
  mimes_types[".mp4"] = "video/mp4";
  mimes_types[".mpeg"] = "video/mpeg";
  mimes_types[".mpg"] = "video/mpeg";
  mimes_types[".mov"] = "video/quicktime";
  mimes_types[".webm"] = "video/webm";
  mimes_types[".flv"] = "video/x-flv";
  mimes_types[".m4v"] = "video/x-m4v";
  mimes_types[".mng"] = "video/x-mng";
  mimes_types[".asx"] = "video/x-ms-asf";
  mimes_types[".asf"] = "video/x-ms-asf";
  mimes_types[".wmv"] = "video/x-ms-wmv";
  mimes_types[".avi"] = "video/x-msvideo";
}

std::string MimeTypes::getType(std::string const &extension) {
  if (!extension.length())
    return "application/octet-stream";
  if (mimes_types.count(extension))
    return mimes_types[extension];
  return "application/octet-stream";
};
