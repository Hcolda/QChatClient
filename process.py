import os

def process(dir = './'):
    for i in os.listdir(dir):
        if os.path.isdir(os.path.join(dir, i)):
            process(os.path.join(dir, i))
        else:
            ext = os.path.splitext(i)[1]
            if ext == '.cpp' or ext == '.hpp' or ext == '.h':
                with open(os.path.join(dir, i), 'rb') as f:
                    text = f.read()
                with open(os.path.join(dir, i), 'wb') as f:
                    f.write("""/**
*    This is a lightweight chat client.
*    Copyright (C) 2022-2025  氢聊-Hcolda.com
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

""".encode())
                    f.write(text[3:])

if __name__ == "__main__":
    process()
