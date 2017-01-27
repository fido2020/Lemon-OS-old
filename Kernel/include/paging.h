#pragma once

extern "C"
void LoadPageDirectory(unsigned int*);

void InitPaging();
void HandlePageFault();