#include <Database/Index.h>

std::ostream& operator<<(std::ostream& outs, const Index& i) {
	for (auto map_it = i.index_.begin(); map_it != i.index_.end(); ++map_it) {
		outs << map_it.Key() << '\n';
		for (auto mmap_it = map_it->begin(); mmap_it != map_it->end(); ++mmap_it) {
			outs << '\t';
			outs << mmap_it.Key() << ": " << util::Print(*mmap_it) << '\n';
		}
	}
	return outs;
}

multi_map::MultiMap<std::string, int64_t>&
Index::operator[](const std::string& key) {
	return index_[key];
}
bool Index::Contains(const std::string& field) { return index_.Contains(field); }
