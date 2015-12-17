#include <stdio.h>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <numeric>


unsigned int getMax(const std::vector<unsigned int> &array) {
	unsigned int maxValue = 0;
	for (auto value : array)
		maxValue = maxValue < value ? value : maxValue;
	return maxValue;
}
unsigned int getPositionIn12(unsigned int arraySize, unsigned int suffix) {
	unsigned int position = (suffix - (suffix + 2) / 3) / 2;
	position += suffix % 3 == 2 ? (arraySize + 1) / 3 : 0;
	return position;
}

unsigned int getByIndex(const std::vector<unsigned int> &array, unsigned int position) {
	return position < array.size() ? array[position] : 0;
}
bool isLower(const std::vector<unsigned int> &array, unsigned int suffix0, unsigned int suffix12, const std::vector<unsigned int> &reversedSuffixArray12) {
	unsigned int offset = suffix12 % 3;
	if (isEqualSubsegments(array, suffix0, suffix12, offset)) {
		unsigned int positionIn12For0, positionIn12For12;
		positionIn12For0 = getPositionIn12((unsigned int)array.size(), suffix0 + offset);
		positionIn12For12 = getPositionIn12((unsigned int)array.size(), suffix12 + offset);
		return getByIndex(reversedSuffixArray12, positionIn12For0) < getByIndex(reversedSuffixArray12, positionIn12For12);
	}
	return isLowerSubsegment(array, suffix0, suffix12, offset);
}

bool isEqualSubsegments(const std::vector<unsigned int> &array, unsigned int firstSubsegment, unsigned int secondSubsegment, unsigned int size) {
	for (unsigned int offset = 0; offset < size; ++offset)
	if (getByIndex(array, firstSubsegment + offset) != getByIndex(array, secondSubsegment + offset))
		return false;
	return true;
}

bool isLowerSubsegment(const std::vector<unsigned int> &array, unsigned int firstSubsegment, unsigned int secondSubsegment, unsigned int size) {
	unsigned int offset;
	for (offset = 0;
		offset < size && getByIndex(array, firstSubsegment + offset) == getByIndex(array, secondSubsegment + offset);
		++offset)
		;
	return offset < size && getByIndex(array, firstSubsegment + offset) < getByIndex(array, secondSubsegment + offset);
}

std::vector<unsigned int> suffixRadixPass(const std::vector<unsigned int> &array, std::vector<unsigned int> &suffixes, unsigned int offset) {
	std::vector<unsigned int> result(suffixes.size());
	std::vector<unsigned int> counts(getMax(array) + 1, 0);
	for (auto suffix : suffixes)
		++counts[getByIndex(array, suffix + offset)];
	for (unsigned int index = 0, sum = 0; index < counts.size(); ++index) {
		std::swap(counts[index], sum);
		sum += counts[index];
	}
	for (auto suffix : suffixes)
		result[counts[getByIndex(array, suffix + offset)]++] = suffix;
	return result;
}

void suffixRadixSort(const std::vector<unsigned int> &array, std::vector<unsigned int> &suffixes, unsigned int suffixSize) {
	for (unsigned int offset = 1; offset <= suffixSize; ++offset)
		suffixes = suffixRadixPass(array, suffixes, suffixSize - offset);
}



std::vector<unsigned int> getSuffixes12(const std::vector<unsigned int> &array) {
	std::vector<unsigned int> suffixes;
	for (unsigned int suffix = 0; suffix < array.size(); ++suffix)
	if (suffix % 3)
		suffixes.push_back(suffix);
	suffixRadixSort(array, suffixes, 3);
	std::vector<unsigned int> sortedNames(suffixes.size(), 1), names(suffixes.size());
	for (unsigned int index = 1; index < suffixes.size(); ++index)
		sortedNames[index] = sortedNames[index - 1] + (int)(!isEqualSubsegments(array, suffixes[index - 1], suffixes[index], 3));
	for (unsigned int index = 0; index < suffixes.size(); ++index)
		names[getPositionIn12((unsigned int)array.size(), suffixes[index])] = sortedNames[index];
	return names;
}

std::vector<unsigned int> getSuffixArray(const std::vector<unsigned int> &array) {
	std::vector<unsigned int> suffixes12 = getSuffixes12(array);
	std::vector<unsigned int> suffixArray12;
	if (getMax(suffixes12) == suffixes12.size()) {
		suffixArray12.resize(suffixes12.size());
		for (unsigned int suffix = 0; suffix < suffixes12.size(); ++suffix)
			suffixArray12[suffixes12[suffix] - 1] = suffix;
	}
	else
		suffixArray12 = getSuffixArray(suffixes12);

	std::vector<unsigned int> reversedSuffixArray12(suffixArray12.size());
	for (unsigned int index = 0; index < suffixArray12.size(); ++index)
		reversedSuffixArray12[suffixArray12[index]] = index + 1;

	std::vector<unsigned int> sortedSuffixes12(suffixes12.size());
	for (unsigned int index = 0; index < array.size(); ++index) {
		if (index % 3) {
			unsigned int positionIn12 = getPositionIn12((unsigned int)array.size(), index);
			sortedSuffixes12[reversedSuffixArray12[positionIn12] - 1] = index;
		}
	}

	std::vector<unsigned int> forRadix0(array), sortedSuffixes0;
	for (unsigned int index = 0; index < array.size(); ++index) {
		if (index % 3)
			forRadix0[index] = reversedSuffixArray12[getPositionIn12((unsigned int)array.size(), index)];
		else
			sortedSuffixes0.push_back(index);
	}
	suffixRadixSort(forRadix0, sortedSuffixes0, 2);

	std::vector<unsigned int> suffixArray;
	unsigned int index0, index12, suffix0, suffix12;
	for (index0 = 0, index12 = 0;
		index0 < sortedSuffixes0.size() && index12 < sortedSuffixes12.size();
		) {
		suffix0 = sortedSuffixes0[index0];
		suffix12 = sortedSuffixes12[index12];
		if (isLower(array, suffix0, suffix12, reversedSuffixArray12)) {
			suffixArray.push_back(suffix0);
			++index0;
		}
		else {
			suffixArray.push_back(suffix12);
			++index12;
		}
	}
	for (; index0 < sortedSuffixes0.size(); ++index0)
		suffixArray.push_back(sortedSuffixes0[index0]);
	for (; index12 < sortedSuffixes12.size(); ++index12)
		suffixArray.push_back(sortedSuffixes12[index12]);

	return suffixArray;
}

std::vector<unsigned int> getSuffixArray(const std::string &str) {
	std::vector<unsigned int> intString(str.size());
	for (unsigned int index = 0; index < str.size(); ++index)
		intString[index] = (int)str[index];
	return getSuffixArray(intString);
}
