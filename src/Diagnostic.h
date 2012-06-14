#ifndef _DIAGNOSTIC_H_
#define _DIAGNOSTIC_H_

enum DiagnosticStatus {
	DIAGNOSTIC_NORMAL,
	DIAGNOSTIC_IS_LIT,
	DIAGNOSTIC_IS_HIT,
	DIAGNOSTIC_TEXTURE_MAPPING,
	DIAGNOSTIC_EDGE_DETECTION,
	DIAGNOSTIC_EDAA_THRESHHOLD
};

const DiagnosticStatus DIAGNOSTIC_STATUS = DIAGNOSTIC_NORMAL;

#endif
