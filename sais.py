import time
import os

def getBuckets(T):
    count = {}
    buckets = {}
    for c in T:
        count[c] = count.get(c, 0) + 1
    start = 0
    for c in sorted(count.keys()):
        buckets[c] = (start, start + count[c])
        start += count[c]
    return buckets


def sais(T):
    t = ["_"] * len(T)
    
    t[len(T) - 1] = "S"
    for i in range(len(T) - 1, 0, -1):
        if T[i-1] == T[i]:
            t[i - 1] = t[i]
        else:
            t[i - 1] = "S" if T[i-1] < T[i] else "L"
    
    buckets = getBuckets(T)

    count = {}
    SA = [-1] * len(T)
    LMS = {}
    end = None
    for i in range(len(T) - 1, 0, -1):
        if t[i] == "S" and t[i - 1] == "L":
            revoffset = count[T[i]] = count.get(T[i], 0) + 1
            SA[buckets[T[i]][1] - revoffset] = i
            if end is not None:
                LMS[i] = end
            end = i

    LMS[len(T) - 1] = len(T) - 1
    count = {}
    for i in range(len(T)):
        if SA[i] >= 0:
            if t[SA[i] - 1] == "L":
                symbol = T[SA[i] - 1]
                offset = count.get(symbol, 0)
                SA[buckets[symbol][0] + offset] = SA[i] - 1
                count[symbol] = offset + 1

    count = {}
    for i in range(len(T) - 1, 0, -1):
        if SA[i] > 0:
            if t[SA[i] - 1] == "S":
                symbol = T[SA[i] - 1]
                revoffset = count[symbol] = count.get(symbol, 0) + 1
                SA[buckets[symbol][1] - revoffset] = SA[i] - 1

    namesp = [-1] * len(T)
    name = 0
    prev = None
    for i in range(len(SA)):
        if t[SA[i]] == "S" and t[SA[i] - 1] == "L":
            if prev is not None and T[SA[prev]:LMS[SA[prev]]] != T[SA[i]:LMS[SA[i]]]:
                name += 1
            prev = i
            namesp[SA[i]] = name

    names = []
    SApIdx = []
    for i in range(len(T)):
        if namesp[i] != -1:
            names.append(namesp[i])
            SApIdx.append(i)

    if name < len(names) - 1:
        names = sais(names)

    names = list(reversed(names))

    SA = [-1] * len(T)
    count = {}
    for i in range(len(names)):
        pos = SApIdx[names[i]]
        revoffset = count[T[pos]] = count.get(T[pos], 0) + 1
        SA[buckets[T[pos]][1] - revoffset] = pos

    count = {}
    for i in range(len(T)):
        if SA[i] >= 0:
            if t[SA[i] - 1] == "L":
                symbol = T[SA[i] - 1]
                offset = count.get(symbol, 0)
                SA[buckets[symbol][0] + offset] = SA[i] - 1
                count[symbol] = offset + 1

    count = {}
    for i in range(len(T) - 1, 0, -1):
        if SA[i] > 0:
            if t[SA[i] - 1] == "S":
                symbol = T[SA[i] - 1]
                revoffset = count[symbol] = count.get(symbol, 0) + 1
                SA[buckets[symbol][1] - revoffset] = SA[i] - 1

    return SA


def search(string, pattern, SA):
    left, right = 0, len(string) - 1
    first_occurrence = -1

    while left <= right:
        mid = (left + right) // 2
        suffix = string[SA[mid] :]
        if suffix.startswith(pattern):
            first_occurrence = mid
            right = mid - 1
        elif pattern < suffix:
            right = mid - 1
        else:
            left = mid + 1

    return first_occurrence

def find_all_occurrences(string, pattern, SA):
    firstOccurrence = search(string, pattern, SA)
    if firstOccurrence == -1:
        return []
    i = firstOccurrence
    occurrences = []
    while(string[SA[i]:].startswith(pattern)):
        occurrences.append(SA[i])
        i += 1
    occurrences.sort()
    return occurrences




def main():
    
    
    #Inicio del tiempo de ejecución del algoritmo
    startTime = time.time()

    #Se crea la ruta donde se lee el libro
    inputBook = os.path.join("Libros", "one.txt")
    
    
    #Variable que guarda el nombre del archivo donde se haya el string que sera usando en la busqueda de ocurrencias dentro del suffix array
    stringPatternInput = "searchString.txt"
    
