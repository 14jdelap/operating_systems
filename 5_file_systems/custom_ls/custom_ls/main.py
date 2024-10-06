import os
import stat
from enum import Enum
import click


class Color(Enum):
    """Colors in ANSI values."""

    PURPLE = "\033[35m"
    RESET = "\033[0m"
    LIGHT_BLUE = "\033[94m"
    BLACK = "\033[30m"


def color_file(name: str, mode: int) -> str:
    """Return directory files with light blue color other files with black."""
    if stat.S_ISDIR(mode):
        return color_text(Color.LIGHT_BLUE, name)
    elif stat.S_ISLNK(mode):
        return color_text(Color.PURPLE, name)
    return name


def color_text(color: Color, name: str) -> str:
    """Return a string"""
    return f"{color.value}{name}{Color.RESET.value}"


@click.command(help="A CLI utility to list files in a directory.")
@click.argument("directory", default=os.getcwd(), type=click.Path(exists=True))
def main(directory: str):
    list_directory(directory=directory)


def list_directory(directory: str):
    """List the files in a directory.

    If a directory is not specified, use the current directory"""
    # Print an entry for the current and parent directory
    for dir_name in [".", ".."]:
        path = os.path.join(directory, dir_name)
        contents = os.lstat(path)
        print(
            f"{contents.st_ino} {contents.st_size:8d}B {color_file(dir_name, contents.st_mode)}"
        )

    # Iterate over the directory contents
    for name in os.listdir(directory):
        path = os.path.join(directory, name)
        contents = os.lstat(path)
        print(
            f"{contents.st_ino} {contents.st_size:8d}B {color_file(name, contents.st_mode)}"
        )
