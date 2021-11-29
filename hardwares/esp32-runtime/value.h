#ifndef __VALUE__H__
#define __VALUE__H__

#ifdef VALUE_MULTI_TYPE_SUPPORT
enum STATES { null = -1, VALUE_TYPE_NUMBER, VALUE_TYPE_TEXT, True, False, Ptr, Array };
#else
#define VALUE_TYPE_NUMBER 0
#define VALUE_TYPE_TEXT 1
#endif

#if __cplusplus >= 201703
inline std::vector<unsigned long> usedPointersList;
#else
static std::vector<unsigned long> usedPointersList;
#endif

#ifndef USE_ARDUINO_STRING
#include <sstream>
#endif
#include <string.h>
#include <vector>

void freeUnusedPointer(long);

#ifdef USE_GMP_LIB
#define NUMBER mpf_class
#include <gmpxx.h>
#include <string.h>
#include <sstream>
#include <iomanip>
#define NUMBER_TO_STRING mpf_class_to_string(this->number).c_str();
#else
#include <BigNumber.h>
#define NUMBER BigNumber
#define NUMBER_TO_STRING number.toString()
#endif

class Value {
#ifdef VALUE_MULTI_TYPE_SUPPORT
	STATES type;
#else
	bool type;
#endif
	NUMBER number;
#ifdef USE_ARDUINO_STRING
#define TEXT String
  String text;
#else
#define TEXT std::string
  std::string text;
#endif
	std::vector<Value> array;
#ifdef USE_GMP_LIB
	std::string mpf_class_to_string(mpf_class data) {
          std::ostringstream s;
	  s << std::setprecision(500) << data;
	  std::string str = s.str();
          return str;
	}
#endif
	public:
#ifdef VALUE_MULTI_TYPE_SUPPORT
		Value() { type = null; }
		Value(STATES a) {
			if (a == null || a == True || a == False || a == Array) {
				type = a;
			} else {
				*this = (int)a;
			}
		}
		Value(bool a) {
			type = a? True:False;
		}
		bool getBool() {
#ifdef VALUE_MULTI_TYPE_SUPPORT
			return type == True? true:false;
#else
			return number != 0? true:false;
#endif
		}
#else
		Value() { type = VALUE_TYPE_NUMBER; }
#endif
		Value(NUMBER data) {
			type = VALUE_TYPE_NUMBER;
			this->number = data;
			this->text = "";
		}

		Value(char c) {
			type = VALUE_TYPE_TEXT;
			text = c;
			number = 0;
		}

#ifdef VALUE_MULTI_TYPE_SUPPORT
		Value(void* a) {
			*this = (long long) a;
			type = Ptr;
			usedPointersList.push_back((unsigned long)a);
		}
#endif

		Value(double data) {
			type = VALUE_TYPE_NUMBER;
			this->number = data;
			this->text = "";
		}

		Value(int data) {
			type = VALUE_TYPE_NUMBER;
			this->number = data;
			this->text = "";
		}

		Value(long long data) {
			char res[25] = {'0', '0', '0', '0', '0', '0',
												'0', '0', '0', '0', '0', '0',
												'0', '0', '0', '0', '0', '0',
												'0', '0', '0', '0', '0', '0'
												, '0'};
			if (data < 0) { res[0] = '-'; data = -data; }
			uint8_t c = 0;
			while(data != 0) {
				res[23 - c++] = (data % 10) + '0';
				data /= 10;
			}
			res[24] = 0;
			*this = NUMBER(res);
		}

		Value(long data) {
			type = VALUE_TYPE_NUMBER;
                        this->text = "";
			this->number = NUMBER(data);
		}

		Value(const char* data) {
			type = VALUE_TYPE_TEXT;
			this->text = data;
			this->number = 0;
		}

    Value(TEXT data) {
			type = VALUE_TYPE_TEXT;
			this->text = data;
			this->number = 0;
		}

		Value(Value* other) {
			type = other->type;
			this->text = other->text;
			this->number = other->number;
			this->array = other->array;
#ifdef VALUE_MULTI_TYPE_SUPPORT
      if (type == Ptr) {
				usedPointersList.push_back(getLong());
			}
#endif
		}

    long getLong() {
#ifdef USE_GMP_LIB
      return this->number.get_si();
#else
      return this->number.toLong();
#endif
    }

#ifdef VALUE_MULTI_TYPE_SUPPORT
		~Value() {
#if __cplusplus >= 201703
			if (type == Ptr) {
				long pointer = getLong();
				std::vector<unsigned long>::iterator tmp = std::find(usedPointersList.begin(), usedPointersList.end(), pointer);
				if (tmp != usedPointersList.end()) {
					usedPointersList.erase(usedPointersList.begin() + std::distance(usedPointersList.begin(), tmp));
					tmp = std::find(usedPointersList.begin(), usedPointersList.end(), getLong());
					if (tmp == usedPointersList.end()) {
						freeUnusedPointer(pointer);
					}
				}
			}
		}
#else
    if (type == Ptr) {
      long pointer = getLong();
      int i = 0;
      bool found = false;
      for (unsigned long tmp : usedPointersList) {
        i++;
        if (tmp == pointer) {
          found = true;
          break;
        }
      }
      if (found) usedPointersList.erase(usedPointersList.begin() + (i - 1));
      found = false;
      for (unsigned long tmp : usedPointersList) {
        if (tmp == pointer) {
          found = true;
          break;
        }
      }
      if (!found) freeUnusedPointer(pointer);
    }
#endif
}
#endif

		Value& operator=(const char* data) {
			type = VALUE_TYPE_TEXT;
			this->text = data;
			this->number = 0;
			return *this;
		}

#ifdef VALUE_MULTI_TYPE_SUPPORT
		Value& operator=(STATES a) {
			if (a == null || a == True || a == False) {
				type = a;
			} else {
				*this = (int)a;
			}
			this->text = "";
			this->number = 0;
			return *this;
		}
#endif

		Value& operator=(TEXT data) {
			type = VALUE_TYPE_TEXT;
			this->text = data;
			this->number = 0;
			return *this;
		}

		Value& operator=(Value other) {
			type = other.type;
			this->number = other.number;
			this->array = other.array;
#ifdef VALUE_MULTI_TYPE_SUPPORT
			if (type == Ptr) {
				usedPointersList.push_back(getLong());
			}
#endif
			this->text = other.text;
			return *this;
		}
		Value& operator=(Value* other) {
			type = other->type;
			this->number = other->number;
			this->array = other->array;
#ifdef VALUE_MULTI_TYPE_SUPPORT
			if (type == Ptr) {
				usedPointersList.push_back(getLong());
			}
#endif
			this->text = other->text;
			return *this;
		}
		Value& operator=(NUMBER n) {
			type = VALUE_TYPE_NUMBER;
			this->number = n;
			this->text = "";
			return *this;
		}

		Value& operator=(int i) {
			type = VALUE_TYPE_NUMBER;
			this->number = i;
			this->text = "";
			return *this;
		}

		TEXT toString() {
			if(type == VALUE_TYPE_TEXT){
				return text;
#ifdef VALUE_MULTI_TYPE_SUPPORT
			} else if (type == VALUE_TYPE_NUMBER) {
				return NUMBER_TO_STRING;
#else
			} else {return NUMBER_TO_STRING;}
#endif
#ifdef VALUE_MULTI_TYPE_SUPPORT
			} else if (type == null) {
				return "null";
			} else if (type == True) {
				return "True";
			} else if (type == Ptr) {
				return NUMBER_TO_STRING;
			} else if (type == Array) {
#ifndef USE_ARDUINO_STRING
				std::ostringstream s;
				s << '[';
				bool isFirst = true;
				for (Value i : array) {
					if (!isFirst)
						s << ", ";
					s << i.toString();
					isFirst = false;
				}
				s << ']';
				return s.str();
#else
        String s;
        s += '[';
				bool isFirst = true;
				for (Value i : array) {
					if (!isFirst)
						s += ", ";
					s += i.toString();
					isFirst = false;
				}
				s += ']';
				return s;
#endif
			} else {
				return "False";
			}
#endif
		}

#ifdef VALUE_MULTI_TYPE_SUPPORT
		STATES
#else
		bool
#endif
		getType() {
			return type;
		}

		NUMBER getNumber() {
			return number;
		}

		TEXT getString() {
			return text;
		}

		bool canNum() {
			if (type == VALUE_TYPE_NUMBER) {
				return true;
			}
#ifdef VALUE_MULTI_TYPE_SUPPORT
			else if (type == True || type == False || type == null) {
				return true;
			}
#endif
			else {
				for (char c : text) {
					if (!isdigit(c)) return false;
				}
				return true;
			}
		}

		Value& toNum() {
			if(type == VALUE_TYPE_NUMBER) return *this;
#ifdef VALUE_MULTI_TYPE_SUPPORT
			if (type == False || type == null) {
				type = VALUE_TYPE_NUMBER;
				number = 0;
				return *this;
			} else if (type == True) {
				type = VALUE_TYPE_NUMBER;
				number = 1;
				return *this;
			}
#endif
			type = VALUE_TYPE_NUMBER;
#ifdef USE_GMP_LIB
			size_t i = 0;
			for(; text[i] != 0; i++){
				if(!isdigit(text[i]) && text[i] != '-' && text[i] != '.') {
					this->number = 0;
					this->text = "";
					return *this;
				}
			}
#endif
			this->number = NUMBER(text.c_str());
			this->text = "";
			return *this;
		}

		Value& toTxt() {
			if(type == VALUE_TYPE_TEXT) return *this;
			*this = toString();
			return *this;
		}

#ifdef VALUE_MULTI_TYPE_SUPPORT
		Value& toPtr() {
			if(type == Ptr) return *this;
			toNum();
			type = Ptr;
			usedPointersList.push_back(getLong());
			return *this;
		}

		Value& toBool() {
			if(type == True || type == False) return *this;
			toNum();
			if (this->number == 0) this->type = False;
			else this->type = True;
			this->text = "";
			this->number = 0;
			return *this;
		}
#endif

		double getDouble() {
#ifdef USE_GMP_LIB
			return this->number.get_d();
#else
			return this->number.toDouble();
#endif
		}

		Value replace(Value from, Value to) {
			toTxt();
#ifndef USE_ARDUINO_STRING
			if(from.toString().empty())
        return text;
    	size_t c = 0;
    	while((c = text.find(from.toString(), c)) != std::string::npos) {
      	text.replace(c, from.toString().length(), to.toString());
  			c += to.toString().length();
    	}
			return text;
#else
      text.replace(from.toString(), to.toString());
      return text;
#endif
		}

		Value find(Value tofind) {
			Value tmp2 = this;
			tmp2.toTxt();
#ifdef USE_ARDUINO_STRING
      return NUMBER((long)tmp2.getString().indexOf(tofind.getString()));
#else
      return NUMBER((long)tmp2.getString().find(tofind.getString()));
#endif
		}

		Value find(Value tofind, Value from) {
			Value tmp2 = this;
			tmp2.toTxt();
#ifdef USE_ARDUINO_STRING
			return NUMBER((long)tmp2.getString().indexOf(tofind.getString(), from.getLong()));
#else
      return NUMBER((long)tmp2.getString().find(tofind.getString(), from.getLong()));
#endif
		}

		Value toUpper() {
			if (type == VALUE_TYPE_NUMBER) {
				toTxt();
				return *this;
			}
#ifdef USE_ARDUINO_STRING
			for (size_t c = 0; c < text.length(); c++) {
#else
			for (size_t c = 0; c < text.size(); c++) {
#endif
				if (text[c] > 96 && text[c] < 123) {
					text[c] -= 32;
				}
			}
			return *this;
		}

		Value toLower() {
			if (type == VALUE_TYPE_NUMBER) {
				toTxt();
				return *this;
			}
#ifdef USE_ARDUINO_STRING
			for (size_t c = 0; c < text.length(); c++) {
#else
			for (size_t c = 0; c < text.size(); c++) {
#endif
				if (text[c] < 91 && text[c] > 64) {
					text[c] += 32;
				}
			}
			return *this;
		}

		Value trimLeft() {
			if (type == VALUE_TYPE_NUMBER) {
				toTxt();
				return *this;
			}
			size_t i = 0;
			while (text[i] == '\n' || text[i] == '\t' || text[i] == ' ') i++;
#ifdef USE_ARDUINO_STRING
      text = text.substring(i);
#else
      text = text.substr(i);
#endif
			return *this;
		}

		Value trimRight() {
			if (type == VALUE_TYPE_NUMBER) {
				toTxt();
				return *this;
			}
#ifdef USE_ARDUINO_STRING
			size_t i = text.length() - 1;
#else
			size_t i = text.size() - 1;
#endif
			while (text[i] == '\n' || text[i] == '\t' || text[i] == ' ') i--;
#ifdef USE_ARDUINO_STRING
      text = text.substring(0, i + 1);
#else
      text = text.substr(0, i + 1);
#endif
			return *this;
		}

		Value trim() {
			if (type == VALUE_TYPE_NUMBER) {
				toTxt();
				return *this;
			}
			trimRight();
			trimLeft();
			return *this;
		}

		Value codePointAt(Value l) {
			Value tmp = l;
			tmp.toNum();
			return (int)text[tmp.getLong()];
		}

		bool endsWith(Value data) {
#ifdef USE_ARDUINO_STRING
      return text.endsWith(data.toString());
#else
			return text.find(data.toString()) == text.size() - data.toString().size();
#endif
		}

		bool startsWith(Value data) {
#ifdef USE_ARDUINO_STRING
      return text.startsWith(data.toString());
#else
			return text.find(data.toString()) == 0;
#endif
		}

		Value& operator+=(Value other) {
#ifdef VALUE_MULTI_TYPE_SUPPORT
#if __cplusplus >= 201703
      if (type == Ptr) {
        std::vector<unsigned long>::iterator tmp = std::find(usedPointersList.begin(), usedPointersList.end(), getLong());
        if (tmp != usedPointersList.end()) {
          usedPointersList.erase(usedPointersList.begin() + std::distance(usedPointersList.begin(), tmp));
        }
      }
#else
    if (type == Ptr) {
      long pointer = getLong();
      int i = 0;
      bool found = false;
      for (unsigned long tmp : usedPointersList) {
        i++;
        if (tmp == pointer) {
          found = true;
          break;
        }
      }
      if (found) usedPointersList.erase(usedPointersList.begin() + (i - 1));
    }
#endif
#endif
			if (((type || other.type) == 0)
#ifdef VALUE_MULTI_TYPE_SUPPORT
				|| ((type == Ptr || type == VALUE_TYPE_NUMBER) && (other.type == Ptr || other.type == VALUE_TYPE_NUMBER))
#endif
		) {
				number += other.number;
#ifdef VALUE_MULTI_TYPE_SUPPORT
				type = VALUE_TYPE_NUMBER;
#endif
			} else {
				toTxt();
				text += other.toString();
			}
			return *this;
		}

		Value operator+(Value other) {
			Value tmp = this;
			tmp += other;
			return tmp;
		}

		Value operator++(int) {
			Value tmp = this;
			operator+=(1);
			return tmp;
		}

		Value operator++() {
			operator+=(1);
			return this;
		}

		Value& operator-=(Value other) {
#ifdef VALUE_MULTI_TYPE_SUPPORT
#if __cplusplus >= 201703
      if (type == Ptr) {
        std::vector<unsigned long>::iterator tmp = std::find(usedPointersList.begin(), usedPointersList.end(), getLong());
        if (tmp != usedPointersList.end()) {
          usedPointersList.erase(usedPointersList.begin() + std::distance(usedPointersList.begin(), tmp));
        }
      }
#else
    if (type == Ptr) {
      long pointer = getLong();
      int i = 0;
      bool found = false;
      for (unsigned long tmp : usedPointersList) {
        i++;
        if (tmp == pointer) {
          found = true;
          break;
        }
      }
      if (found) usedPointersList.erase(usedPointersList.begin() + (i - 1));
    }
#endif
#endif
			if (((type || other.type) == 0)
#ifdef VALUE_MULTI_TYPE_SUPPORT
				|| ((type == Ptr || type == VALUE_TYPE_NUMBER) && (other.type == Ptr || other.type == VALUE_TYPE_NUMBER))
#endif
		) {
				number -= other.number;
#ifdef VALUE_MULTI_TYPE_SUPPORT
				type = VALUE_TYPE_NUMBER;
#endif
			} else {
				toTxt();
				replace(other.toString(), "");
			}
			return *this;
		}

		Value operator-(Value other) {
			Value tmp = this;
			tmp -= other;
			return tmp;
		}

		Value operator--(int) {
			Value tmp = this;
			operator-=(1);
			return tmp;
		}

		Value operator--() {
			operator-=(1);
			return this;
		}

		Value& operator*=(Value other) {
			if ((type || other.type) == 0) {
				number *= other.number;
			} else if (type == VALUE_TYPE_NUMBER && other.type == VALUE_TYPE_TEXT) {
#ifdef USE_ARDUINO_STRING
        text = "";
		    for (NUMBER i = 0; i < number; i++) text += other.text;
				type = VALUE_TYPE_TEXT;
#else
		    std::ostringstream os;
		    for(NUMBER i = 0; i < number; i++) os << other.text;
		    text = os.str();
				type = VALUE_TYPE_TEXT;
#endif
				number = 0;
			} else if (type == VALUE_TYPE_TEXT && other.type == VALUE_TYPE_NUMBER) {
#ifdef USE_ARDUINO_STRING
        text = "";
		    for (NUMBER i = 0; i < other.number; i++) text += text;
				type = VALUE_TYPE_TEXT;
#else
		    std::ostringstream os;
		    for(NUMBER i = 0; i < other.number; i++) os << text;
		    text = os.str();
				type = VALUE_TYPE_TEXT;
#endif
				number = 0;
			} else {
				toNum();
#ifdef USE_ARDUINO_STRING
        text = "";
		    for (NUMBER i = 0; i < number; i++) text += other.text;
				type = VALUE_TYPE_TEXT;
#else
		    std::ostringstream os;
		    for(NUMBER i = 0; i < number; i++) os << other.text;
		    text = os.str();
				type = VALUE_TYPE_TEXT;
#endif
				number = 0;
			}
			return *this;
		}

		Value operator*(Value other) {
			Value tmp = this;
			tmp *= other;
			return tmp;
		}

		Value& operator/=(Value other) {
			if ((type || other.type) == 0) {
				number /= other.number;
			} else {
				toNum();
				Value tmp = other;
				tmp.toNum();
				number /= tmp.number;
			}
			return *this;
		}

		Value operator/(Value other) {
			Value tmp = this;
			tmp /= other;
			return tmp;
		}

		Value& operator%=(Value other) {
			if ((type || other.type) == 0) {
#ifdef USE_GMP_LIB
				number = number.get_si() % other.number.get_si();
#else
				number %= other.number;
#endif
			} else {
				toNum();
				Value tmp = other;
				tmp.toNum();
#ifdef USE_GMP_LIB
				number = number.get_si() % tmp.number.get_si();
#else
				number %= tmp.number;
#endif
			}
			return *this;
		}

		Value operator%(Value other) {
			if (other == 1) {
#ifdef USE_GMP_LIB
				return Value(number - floor(number));
#else
#ifndef USE_ARDUINO_STRING
        std::string tmp = number.toString();
				return number - NUMBER(tmp.substr(0, tmp.find(".")).c_str());
#else
        String tmp = number.toString();
				return number - NUMBER(tmp.substring(0, tmp.indexOf(".")));
#endif
#endif
			}
			Value tmp = this;
			tmp %= other;
			return tmp;
		}

		Value operator[](Value i) {
			Value tmp = this;
			tmp.toTxt();
#ifdef USE_ARDUINO_STRING
			if (tmp.text.length() > i.getLong()) {
#else
			if (tmp.text.size() > i.getLong()) {
#endif
				return Value(tmp.text[i.getLong()]);
			}
			return Value(-1);
		}

		Value operator[](int i) {
			Value tmp = this;
			tmp.toTxt();
#ifdef USE_ARDUINO_STRING
			if (tmp.text.length() > i) {
#else
			if (tmp.text.size() > i) {
#endif
				return Value(tmp.text[i]);
			}
			return Value(-1);
		}

		char charAt(size_t p) {
			return toString()[p];
		}

		bool operator==(Value other) {
			return strictEquals(other);
		}

		bool operator!=(Value other) {
			return !strictEquals(other);
		}

		bool equals(Value other) {
#ifdef VALUE_MULTI_TYPE_SUPPORT
			return toString() == other.toString() || (type == VALUE_TYPE_NUMBER && (other.type == True || other.type == False) && (other.getBool() == this->number)) || ((type == True || type == False) && other.type == VALUE_TYPE_NUMBER && (getBool() == other.number));
#else
			return toString() == other.toString();
#endif
		}

		bool strictEquals(Value other) {
			return type == other.type && equals(other);
		}

		Value substring(Value other) {
			if (type == VALUE_TYPE_NUMBER) toTxt();
#ifdef USE_ARDUINO_STRING
			return text = text.substring(other.getLong());
#else
			return text = text.substr(other.getLong());
#endif
		}

		Value substring(Value v1, Value v2) {
			if (type == VALUE_TYPE_NUMBER) toTxt();
#ifdef USE_ARDUINO_STRING
			return text = text.substring(v1.getLong(), v2.getLong());
#else
			return text = text.substr(v1.getLong(), v2.getLong() - v1.getLong());
#endif
		}

		Value reverse() {
			if (type == VALUE_TYPE_NUMBER) {
				toTxt();
			}
			if (text == "") {
				return *this;
			}
#ifdef USE_ARDUINO_STRING
			size_t len = text.length() - 1;
#else
			size_t len = text.size() - 1;
#endif
			for (size_t i = 0; i < (len / 2) + 1; i++) {
				char tmp = text[len - i];
				text[len - i] = text[i];
				text[i] = tmp;
			}
			return *this;
		}

		Value operator&&(Value other) {
#ifdef VALUE_MULTI_TYPE_SUPPORT
			Value tmp = this;
			Value tmpOther = other;
			tmp.toBool();
			tmpOther.toBool();
			return tmp.getBool() && tmpOther.getBool();
#else
			return number != 0 && other.number != 0;
#endif
		}

		Value operator||(Value other) {
#ifdef VALUE_MULTI_TYPE_SUPPORT
			return getBool() || other.getBool();
#else
			return number != 0 || other.number != 0;
#endif
		}

		Value operator!() {
#ifdef VALUE_MULTI_TYPE_SUPPORT
			if ((type != null && (type == VALUE_TYPE_NUMBER && number != 0)) || type == True) {
				return false;
			}
			return true;
#else
			return number == 0;
#endif
		}

		Value operator~() {
			Value tmp = this;
			tmp.toNum();
			return (int)~tmp.getLong();
		}

		Value operator&(Value other) {
			Value tmp = this;
			tmp.toNum();
			Value tmp2 = other;
			tmp2.toNum();
#ifdef VALUE_MULTI_TYPE_SUPPORT
			if ((getType() == True || getType() == False) && (other.getType() == False || other.getType() == True))
				return (bool)(tmp.getLong() & tmp2.getLong());
#endif
			return (int)(tmp.getLong() & tmp2.getLong());
		}

		Value operator|(Value other) {
			Value tmp = this;
			tmp.toNum();
			Value tmp2 = other;
			tmp2.toNum();
#ifdef VALUE_MULTI_TYPE_SUPPORT
			if ((getType() == True || getType() == False) && (other.getType() == False || other.getType() == True))
				return (bool)(tmp.getLong() | tmp2.getLong());
#endif
			return (int)(tmp.getLong() | tmp2.getLong());
		}

		Value pow(Value a) {
#ifdef USE_GMP_LIB
			mpf_pow_ui(number.get_mpf_t(), number.get_mpf_t(), a.number.get_ui());
#else
			number = number.pow(a.number);
#endif
			return this;
		}

		Value append(Value other) {
#ifdef VALUE_MULTI_TYPE_SUPPORT
			if (type == Array) {
				array.push_back(other);
			} else
#endif
      if (type == VALUE_TYPE_TEXT) {
				text += other.toString();
			} else if (other.type == VALUE_TYPE_TEXT) {
				toTxt();
				text += other.getString();
			}
#ifdef VALUE_MULTI_TYPE_SUPPORT
       else {
				array.push_back(this);
				type = Array;
				array.push_back(other);
			}
#endif
			return this;
		}

		Value insert(size_t point, Value other) {
#ifdef VALUE_MULTI_TYPE_SUPPORT
			if (type == Array) {
				if(array.size() < (point + 1)) array.resize(point);
				array.insert(array.begin() + point, other);
			} else
#endif
      if (type == VALUE_TYPE_TEXT) {
#ifdef USE_ARDUINO_STRING
        text = text.substring(0, point) + other.toString() + text.substring(point);
#else
				text.insert(point, other.toString());
#endif
			} else if (other.type == VALUE_TYPE_TEXT) {
				toTxt();
#ifdef USE_ARDUINO_STRING
        text = text.substring(0, point) + other.getString() + text.substring(point);
#else
				text.insert(point, other.getString());
#endif
			}
#ifdef VALUE_MULTI_TYPE_SUPPORT
      else {
				array.push_back(this);
				type = Array;
				if(array.size() < (point + 1)) array.resize(point);
				array.insert(array.begin() + point, other);
			}
#endif
			return this;
		}

		Value set(size_t point, Value other) {
#ifdef VALUE_MULTI_TYPE_SUPPORT
			if (type == Array) {
				if(array.size() < (point + 1)) array.resize(point + 1);
				array[point] = other;
			} else
#endif
      if (type == VALUE_TYPE_TEXT) {
				text[point] = other.toString()[0];
			} else if (other.type == VALUE_TYPE_TEXT) {
				toTxt();
				text[point] = other.getString()[0];
			}
#ifdef VALUE_MULTI_TYPE_SUPPORT
      else {
				array.push_back(this);
				type = Array;
				if(array.size() < (point + 1)) array.resize(point + 1);
				array[point] = other;
			}
#endif
			return this;
		}

		Value get(size_t point) {
#ifdef VALUE_MULTI_TYPE_SUPPORT
			if (type == Array) {
				return array[point];
			} else
#endif
      if (type == VALUE_TYPE_TEXT) {
				return text[point];
			} else {
				Value tmp = this;
				return tmp.toString()[point];
			}
		}

   Value length() {
#ifdef VALUE_MULTI_TYPE_SUPPORT
     if (type == Array) {
        return (long) array.size();
      } else
#endif
      if (type == VALUE_TYPE_TEXT) {
#ifndef USE_ARDUINO_STRING
        return (long) text.size();
#else
        return (long) text.length();
#endif
      }
      return -1;
   }

		Value operator^(Value other) {
			Value tmp = this;
			tmp.toNum();
			Value tmp2 = other;
			tmp2.toNum();
#ifdef VALUE_MULTI_TYPE_SUPPORT
			if ((getType() == True || getType() == False) && (other.getType() == False || other.getType() == True))
				return (bool)(tmp.getLong() ^ tmp2.getLong());
#endif
			return (int)(tmp.getLong() ^ tmp2.getLong());
		}

		Value operator-() {
			Value tmp = this;
			tmp.toNum();
#ifdef USE_GMP_LIB
			return mpf_class(NUMBER(-1) * tmp.getNumber());
#else
			return NUMBER(-1) * tmp.getNumber();
#endif
		}

		Value operator<<(Value other) {
			Value tmp = this;
			tmp.toNum();
			Value tmp2 = other;
			tmp2.toNum();
#ifdef USE_GMP_LIB
			return tmp.getLong() << tmp2.getLong();
#else
			return tmp.getNumber() << tmp2.getNumber();
#endif
		}

		Value operator>>(Value other) {
			Value tmp = this;
			tmp.toNum();
			Value tmp2 = other;
			tmp2.toNum();
#ifdef USE_GMP_LIB
			return tmp.getLong() >> tmp2.getLong();
#else
			return tmp.getNumber() >> tmp2.getNumber();
#endif
		}

#ifdef NUMBER_COMPARISON_OPERATORS
		bool operator<(Value other) {
			if (other.type == VALUE_TYPE_TEXT || type == VALUE_TYPE_TEXT) return false;
			return number < other.number;
		}
		bool operator>(Value other) {
			if (other.type == VALUE_TYPE_TEXT || type == VALUE_TYPE_TEXT) return false;
			return number > other.number;
		}
		bool operator<=(Value other) {
			if (other.type == VALUE_TYPE_TEXT || type == VALUE_TYPE_TEXT) return false;
			return number <= other.number;
		}
		bool operator>=(Value other) {
			if (other.type == VALUE_TYPE_TEXT || type == VALUE_TYPE_TEXT) return false;
			return number >= other.number;
		}
    static void print(Value v) {
#ifndef USE_ARDUINO_STRING
      std::cout << v.toString();
#else
      Serial.println(v.toString());
#endif
    }
#endif
};

#ifndef USE_ARDUINO_STRING
inline std::ostream &operator<<(std::ostream &s, Value *v) {
	return s << v->toString();
}

inline std::ostream &operator<<(std::ostream &s, const Value &v) {
	return s << (Value*) &v;
}

inline std::istream& operator>>(std::istream &in, Value &val) {
  std::string a;
  in >> a;
  val = a;
  return in;
}
#endif

#endif
