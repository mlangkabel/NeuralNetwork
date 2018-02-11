#include "utility/TextAccess.h"

#include <fstream>

#include "utility/logging.h"

std::shared_ptr<TextAccess> TextAccess::createFromFile(const std::string& filePath)
{
	std::shared_ptr<TextAccess> result(new TextAccess());

	result->m_text = readFile(filePath);
	result->m_filePath = filePath;

	return result;
}

std::shared_ptr<TextAccess> TextAccess::createFromString(const std::string& text, const std::string& filePath)
{
	std::shared_ptr<TextAccess> result(new TextAccess());

	result->m_text = text;
	result->m_filePath = filePath;

	return result;
}

std::string TextAccess::getText() const
{
	return m_text;
}

std::string TextAccess::getFilePath() const
{
	return m_filePath;
}

std::string TextAccess::readFile(const std::string& filePath)
{
	std::string result = "";

	{
		std::ifstream srcFile;
		srcFile.open(filePath);
		if (srcFile.fail())
		{
			LOG_ERROR("Could not open file \"" + filePath + "\".");
			return result;
		}

		bool fist = true;
		while (!srcFile.eof())
		{
			if (!fist)
			{
				result += '\n';
			}
			fist = false;

			std::string line;
			std::getline(srcFile, line);
			result += line;
		}
		srcFile.close();
	}

	return result;
}

TextAccess::TextAccess()
	: m_filePath("")
{
}
