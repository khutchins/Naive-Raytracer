#ifndef _DIAGNOSTIC_H_
#define _DIAGNOSTIC_H_

typedef enum DiagnosticStatus {
	NORMAL,
	IS_LIT,
	IS_HIT,
	TEXTURE_MAPPING
};

const DiagnosticStatus DIAGNOSTIC_STATUS = TEXTURE_MAPPING;

#endif