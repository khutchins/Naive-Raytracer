#ifndef _DIAGNOSTIC_H_
#define _DIAGNOSTIC_H_

enum DiagnosticStatus {
	NORMAL,
	IS_LIT,
	IS_HIT,
	TEXTURE_MAPPING
};

const DiagnosticStatus DIAGNOSTIC_STATUS = NORMAL;

#endif