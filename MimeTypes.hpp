#ifndef MIMETYPES_HPP
# define MIMETYPES_HPP

# include <map>
# include <string>


class MimeTypes {
 public:
  MimeTypes();
  std::string getType(std::string const &extension);

 private:
  std::map<std::string, std::string> mimes_types;
};

#endif
