#pragma once



template<typename STRT>
struct sfit {
	unsigned long length;
	long long pos = 0;
	STRT* pt;
	sfit(const unsigned long& char_origin_length, STRT* origin_ptr) :length(char_origin_length/sizeof(STRT)), pt(origin_ptr) { ; }
	~sfit() { ; }
	bool operator>(STRT*& gets) {
		gets = pt++;
		++pos;
		if (pos > length) {
			--pos;
			--pt;
			return false;
		}
		return true;
	}
	bool operator<(STRT*& gets) {
		gets = --pt;
		--pos;
		if (pos < 0) {
			++pos;
			++pt;
			return false;
		}
		return true;
	}
};

template<typename STRT>
inline
unsigned long
char_sz(const unsigned long& strt_sz) {
	return strt_sz * (unsigned long)sizeof(STRT);
}