from googletrans import Translator
import os

f = open("transtmp.txt", "r")
word = f.read().split()[0]
f.close()

translator = Translator(service_urls=[
  'translate.google.com'
])

to_eng = {
     "а": "a",
     "б": "b",
     "ц": "c",
     "д": "d",
     "е": "e",
     "ф": "f",
     "г": "g",
     "х": "h",
     "и": "i",
     "ж": "j",
     "к": "k",
     "л": "l",
     "м": "m",  "н": "n",  "о": "o",  "п": "p",
     "п": "q",  "р": "r",  "с": "s",  "т": "t",
     "ю": "u",  "в": "v",  "ш": "w",  "щ": "x",
     "у": "y",  "з": "z",  "ь": "1",  "ъ": "2",
     "э": "3",  "ч": "4",  "я": "5",  "ы": "6",
     "й": "7"
}

f = open('transtmp.txt','w')

trword = translator.translate(word, dest='ru')

coded_word = str()
for i in trword.text:
  if to_eng.get(i.lower()) != None:
    coded_word += to_eng[i.lower()]
  else: coded_word += i # must be ' , and etc. 
f.write(coded_word)

f.close()
