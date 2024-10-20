CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -g 
SRC = *.cc
SRC_TEST = tests/*.cc
OBJ = $(patsubst %.cc, %.o, $(wildcard $(SRC)))
OBJ_TEST = $(patsubst %.cc, %.o, $(wildcard $(SRC_TEST)))

.PHONY: all clean test gtest build

NAME:=$(shell uname -s)
ifeq ($(NAME), Linux)
		LDFLAGS= -lgtest -lgmock -lpthread
else ifeq ($(NAME), Darwin)
		LDFLAGS= -lgtest -lgmock 
endif

all: clean build
 
build:
		@- mkdir build
		cd build/ && cmake -DCMAKE_BUILD_TYPE=Release ../SmartCalc_2/CMakeLists.txt
		cd build/ && make
		chmod 755 build/SmartCalc_2.app
b2: 
		@-mkdir b2
		cd b2/ && cmake ../SmartCalc_2/CMakeLists.txt
		cd b2/ && cmake --build . --config Release
		chmod 755 b2/SmartCalc_2.app
open:
		./build/SmartCalc_2.app/Contents/MacOS/SmartCalc_2

install: clean build
		@-mkdir ../../Calculator_v2.0
		cp -rf build/SmartCalc_2.app ../../Calculator_v2.0/SmartCalc_2.app
uninstall: 
		rm -rf ../../Calculator_v2.0

dvi:
		-doxygen Doxyfile -w html 
		open html/index.html
dviclean:
		rm -rf html/ latex/

dist:
		tar cvf ../../SmartCalc-2.0.tar SmartCalc_2/*.ui SmartCalc_2/*.h SmartCalc_2/*.cc SmartCalc_2/*.txt ./Doxyfile ./description.h ./Makefile *.cc *.h
dist2: build 
		tar cvf  ../../Smartalc-2.0.tar build/SmartCalc_2.app
distclean:
		rm -rf ../../SmartCalc-2.0.tar

test: $(OBJ) $(OBJ_TEST)
		$(CXX) $^ $(LDFLAGS) -o $@
		./test
%.o: %.cc %.h
		echo $@
		$(CXX) $(CXXFLAGS) -c $(filter-out %.h, $^)  -o $@
clean:
		@rm -rf $(OBJ) $(OBJ_TEST)
		@rm -rf test gtest test.dSYM/ 
		@rm -rf *.info *.gcno *.gcda report
		@rm -rf test/*.info tests/*.gcda tests/*.gcno
		@rm -rf build

leaks: clean test
		leaks -atExit -- ./test
san: clean add_sanitaizer test
val: clean test
		valgrind --tool=memcheck --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./test

leak_app: 
		leaks -atExit -- ./build/SmartCalc_2.app/Contents/MacOS/SmartCalc_2

check:
		@cp ../materials/linters/.clang-format ./
		clang-format -n *.cc *.h tests/*.cc tests/*.h SmartCalc_2/*.cc SmartCalc_2/*.h  
		@rm -rf .clang-format
check2:
		@cp ../materials/linters/.clang-format ./
		clang-format -i  *.cc *.h tests/*.cc tests/*.h SmartCalc_2/*.cc SmartCalc_2/*.h 
		@rm -rf .clang-format
start: 
		open SmartCalc_2/CMakeLists.txt -a "Qt Creator.app"
gcov_report: clean add_coverage $(OBJ) $(OBJ_TEST)
		@$(CXX) $(CXXFLAGS) $(OBJ) $(OBJ_TEST) $(LDFLAGS) -o test
		@./test
		@mkdir report
		lcov --ignore-errors mismatch --ignore-errors empty --no-external --no-recursion -t "test" -q  -o test.info -c -d .
		lcov --r test.info "s21_deposit_model.cc" "s21_controller.cc" "s21_credit_model.cc" -o test.info
		genhtml --ignore-errors mismatch -o report test.info
		open report/index.html
		rm -rf *.gcda *.gcno *.out
add_coverage:
	$(eval CXXFLAGS += -fprofile-arcs -ftest-coverage -O0)
add_sanitaizer:
	$(eval CXXFLAGS += -fsanitize=address)
	$(eval LDFLAGS +=	-fsanitize=address)


brew:
		/bin/bash -c "cd /opt/goinfre/${USER} && git clone https://github.com/Homebrew/brew homebrew"
		printf "eval \"%c(/opt/goinfre/${USER}/homebrew/bin/brew shellenv)\"\n\
brew update --force --quiet\n\
chmod -R go-w \"%c(brew --prefix)/share/zsh\"" "$$" "$$" >> /Users/${USER}/.zprofile \
&& export PATH=/opt/goinfre/${USER}/homebrew/bin:${PATH} && source ~/.zshrc
install_gtest: 
		/bin/zsh -c "cd /opt/goinfre/${USER} \
		&& git clone https://github.com/google/googletest.git \
		&& cd googletest && mkdir build && cd build && cmake -DCMAKE_CXX_COMPILER:STRING="/usr/local/bin/g++" .. \
		&& make && cp -a lib/*.a ../../homebrew/lib \
		&& cp -a ../googletest/include/gtest ../../homebrew/include"
