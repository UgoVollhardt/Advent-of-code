#ifndef ELEMENTFILESYSTEM_ADVENTOFCODE_H
#define ELEMENTFILESYSTEM_ADVENTOFCODE_H

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class ElemFilesystem {
 public:
  enum class ElemType { kFile, kDir };

  ElemFilesystem(std::string name, ElemType type,
                 std::shared_ptr<ElemFilesystem> parent = nullptr,
                 size_t size = 0) {
    name_ = name;
    type_ = type;
    parent_ = parent;
    size_ = size;
  }

  ~ElemFilesystem() = default;

  void addChild(std::shared_ptr<ElemFilesystem> child) {
    children_[child->getName()] = child;
  }

  size_t getSize() {
    if (type_ == ElemType::kFile) {
      return size_;
    } else {
      size_t sum = 0;
      for (auto &i : children_) {
        sum += i.second->getSize();
      }
      return sum;
    }
  }

  std::string getName() const { return name_; }

  ElemType getType() const { return type_; }

  std::shared_ptr<ElemFilesystem> getChild(std::string name) {
    return children_[name];
  }

  std::shared_ptr<ElemFilesystem> getParent() { return parent_; }

  std::vector<std::shared_ptr<ElemFilesystem>> getFolders(
      bool recursive = false) {
    std::vector<std::shared_ptr<ElemFilesystem>> out;
    for (auto &i : children_) {
      if (i.second->getType() == ElemType::kDir) {
        out.push_back(i.second);
        if (recursive) {
          auto childrenFolders = i.second->getFolders(recursive);
          out.insert(out.end(), childrenFolders.begin(), childrenFolders.end());
        }
      }
    }
    return out;
  }

 private:
  std::string name_;
  ElemType type_;
  size_t size_;
  std::shared_ptr<ElemFilesystem> parent_;
  std::map<std::string, std::shared_ptr<ElemFilesystem>> children_;
};

#endif