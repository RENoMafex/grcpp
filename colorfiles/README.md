# grcpp colorfiles
Colorfiles for [grcpp].

>[!NOTE]
> This README is **NOT** towards end users, but developers who want to contribute to [grcpp].</br>
> In fact this whole repository is not for end users.

## Contributing is *very* **EASY**!
#### Colorfiles for new programs
If you want to add a colorfile for a program, that is not existing yet, just contribute the **conf.*programname*** file and change the **grcpp.conf** file accordingly. These pull requests will most likely be merged, even if that program is not very popular.

#### Working on existing colorfiles
If you want to change an already existing file, that change should serve any purpose (maybe it makes the regexes more precise or adds functionality). If your pull request only changes colors based off some personal preference, that pull request will most likely not be merged.

## How [grcpp] uses this repo
**Grcpp** uses this repo as a submodule and pulls it through a GitHub Action once per day.

## License disclaimer
Most of these colorfiles are just copied from [grc](https://github.com/garabik/grc), which is licensed under the GNU General Public License. I decided to put this repo under the *unlicense* anyway, because all files inside this repo mostly consist of regexes, which are not that complex (for regexes), so i don't think, that the contents of this repo are even complex/unique enough to put under any license, that is restrictive in any way.

[grcpp]: https://github.com/RENoMafex/grcpp
