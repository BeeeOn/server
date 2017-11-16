# Functional tests

This directory contains functional tests of the server. Each test suite
usually require the server or some its parts to be started manually
prior to testing.

The tests are written in Python 3. Required Python libraries:

* `unittest-tap-reporting`
* see `t0000-selftest.py` for more required libraries

Optional libraries:

* `selenium` (OAuth2 tests)
* `phantomjs` (OAuth2 tests)
* `python-selenium` (OAuth2 tests)
* `python-oauth2client` (OAuth2 tests)


## Install unittest-tap-reporting

Via RPM:

```
$ git clone https://github.com/vit1251/unittest-tap-reporting.git
$ (cd unittest-tap-reporting && python3 setup.py bdist --formats=rpm)
# rpm -i unittest-tap-reporting/dist/unittest-tap-reporting-0.1-1.noarch.rpm
```

Via easy_install:

```
# easy_install unittest-tap-reporting
```

## Install phantomjs

If not found in your distribution, this is the alternative way:

```
$ wget https://bitbucket.org/ariya/phantomjs/downloads/phantomjs-2.1.1-linux-x86_64.tar.bz2
$ tar -xf phantomjs-2.1.1-linux-x86_64.tar.bz2
$ sudo cp phantomjs-2.1.1-linux-x86_64/bin/phantomjs /usr/local/bin/phantomjs
```

It might require freetype and fontconfig packages.


## Run tests

Just start all the `t####-*` files:

```
$ for t in t[0-9]* ui/t[0-9]*; do
	./${t}
  done
```


## Run tests using prove

```
$ prove t[0-9]* ui/t[0-9]*
```


## Run single test

Simply execute the selected testing script:

```
$ ./t0000-selftest.py
```

To execute a selected subtest, you can specify the subtest on
the command line like:

```
$ ./t0000-selftest.py TestSelf.test1_self
```


== New test template ==

See `t0000-selftest.py` that test whether the testing environment is
set up correctly. Use the `t0000-selftest.py` as the starting template
for new tests.
