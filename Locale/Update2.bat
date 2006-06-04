@echo off
FOR %%a IN (*.po) DO IF NOT %%a==Template.po IF NOT %%a==Ignore.po msgmerge -U -v --backup=off %%a Template.po
pause
