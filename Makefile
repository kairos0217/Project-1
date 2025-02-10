CFLAGS = -Wall -Werror -g
CC = gcc $(CFLAGS)
AN = proj1
SHELL = /bin/bash
CWD = $(shell pwd | sed 's/.*\///g')

.PHONY: test clean clean-tests

concordance_check: concordance_check.o concordancer.o
	$(CC) -o $@ $^

concordance_check.o: concordance_check.c
	$(CC) -c $<

concordancer.o: concordancer.c concordancer.h
	$(CC) -c $<

test-setup:
	@chmod u+x testius
	@rm -f test_concordancer.txt test_concordancer_large.txt

ifdef testnum
test: concordance_check test-setup
	./testius test_cases/tests.json -v -n "$(testnum)"
else
test: concordance_check test-setup
	./testius test_cases/tests.json
endif

zip: clean clean-tests
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)" -x "$(CWD)/test_cases/*" "$(CWD)/testius"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi

clean:
	rm -f *.o concordance_check

clean-tests:
	rm -rf test_results/
	rm -f test_concordancer.txt test_concordancer_large.txt
