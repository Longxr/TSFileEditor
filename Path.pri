
CONFIG(release, debug|release) {
    BuildType=release
    CONFIG += warn_off
} else {
    BuildType=debug
    DEFINES += __DEBUG
}

#DESTDIR         = $$PWD/../RunImage/$$BuildType
DESTDIR = $$absolute_path($$PWD/bin/)



