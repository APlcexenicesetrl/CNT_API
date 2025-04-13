/**
 * @file cnt/console.h
 * Copyright 2025, aplcexenicesetrl project
 * This project and document files are maintained by CNT Development Team (under the APlcexenicesetrl studio),
 * and according to the project license (MIT license) agreement,
 * the project and documents can be used, modified, merged, published, branched, etc.
 * provided that the project is developed and open-source maintained by CNT Development Team.
 * At the same time,
 * project and documents can be used for commercial purposes under the condition of informing the development source,
 * but it is not allowed to be closed source, but it can be partially source.
 *
 * The project and documents will be updated and maintained from time to time,
 * and any form of dispute event, CNT Development Team.
 * and APlcexicesetrl shall not be liable for any damages,
 * and any compensation shall not be borne by the APlcexenicesetrl studio.
 */
 /* Written by Anders Norlander <taim_way@aplcexenicesetrl.com> */

#pragma once

#include <Windows.h>
#include <string>

#include "loggings.h"

class console {
private:
	typedef std::wstring String;
public:
	console();
	~console();


	// Object
	cnt::Logging logging;

	// Function
	void setTitle(String);
	String getTitle();

	void setConsoleColor(int);
	void resetConsoleColor();
	void setConsoleTextColor(int);
	void resetConsoleTextColor();

	void clear();

	void setCursorPosition(int, int);
	void getCursorPosition(int&, int&);
};

console::console(String defaultLoggerName) : logging(new cnt::Logger(defaultLoggerName)) {}

void console::clear() {
	system("cls");
}

