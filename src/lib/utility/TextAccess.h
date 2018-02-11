#ifndef TEXT_ACCESS_H
#define TEXT_ACCESS_H

#include <memory>
#include <string>
#include <vector>

class TextAccess
{
public:
	static std::shared_ptr<TextAccess> createFromFile(const std::string& filePath);
	static std::shared_ptr<TextAccess> createFromString(const std::string& text, const std::string& filePath = "");

	std::string getText() const;
	std::string getFilePath() const;

private:
	static std::string readFile(const std::string& filePath);

	TextAccess();
	TextAccess(const TextAccess&) = delete;
	TextAccess operator=(const TextAccess&) = delete;

	std::string m_text;
	std::string m_filePath;
};

#endif // TEXT_ACCESS_H
