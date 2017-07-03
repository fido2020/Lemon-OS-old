struct multiboot_info {
	uint32_t flags;
	uint32_t memoryLo;
	uint32_t memoryHi;
	uint32_t bootDevice;
	uint32_t cmdline;
	uint32_t modsCount;
	uint32_t modsAddr;
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
	uint32_t mmapLength;
	uint32_t mmapAddr;
	uint32_t drivesLength;
	uint32_t drivesAddr;
	uint32_t configTable;
	uint32_t bootloaderName;
	uint32_t apmTable;

	uint32_t vbeControlInfo;
	uint32_t vbeModeInfo;
	uint16_t vbeMode;
	uint16_t vbeInterfaceSeg;
	uint16_t vbeInterfaceOff;
	uint16_t vbeInterfaceLen;

	uint64_t framebufferAddr;
	uint32_t framebufferPitch;
	uint32_t framebufferWidth;
	uint32_t framebufferHeight;
	uint8_t framebufferBpp;
	uint8_t framebufferType;
}__attribute__ ((packed));

struct video_mode_info{
    uint8_t type;
    uint64_t addr;
    uint32_t width;
    uint32_t height;
    uint8_t bpp;
    uint32_t pitch;
};