/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2013 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  UsbOcUpdateDxe.c

Abstract:

--*/
#include "UsbOcUpdateDxe.h"

#include <Library/UbaUsbOcUpdateLib.h>
#include <PchLimits.h>
#include <PchPolicyCommon.h>
#include <Library/PchInfoLib.h>


PCH_USB_OVERCURRENT_PIN TypeNeonCityEPECBUsb20OverCurrentMappings[PCH_MAX_USB2_PORTS] = {
                          PchUsbOverCurrentPin0,
                          PchUsbOverCurrentPin1,
                          PchUsbOverCurrentPin1,
                          PchUsbOverCurrentPin2,
                          PchUsbOverCurrentPin3,
                          PchUsbOverCurrentPin3,
                          PchUsbOverCurrentPin7,
                          PchUsbOverCurrentPin7,
                          PchUsbOverCurrentPin6,
                          PchUsbOverCurrentPin4,
                          PchUsbOverCurrentPin6,
                          PchUsbOverCurrentPin4,
                          PchUsbOverCurrentPin5,
                          PchUsbOverCurrentPin4,
                          PchUsbOverCurrentPinSkip,
                          PchUsbOverCurrentPinSkip
                       };

PCH_USB_OVERCURRENT_PIN TypeNeonCityEPECBUsb30OverCurrentMappings[PCH_MAX_USB3_PORTS] = {
                          PchUsbOverCurrentPin0,
                          PchUsbOverCurrentPin1,
                          PchUsbOverCurrentPin1,
                          PchUsbOverCurrentPin2,
                          PchUsbOverCurrentPin3,
                          PchUsbOverCurrentPin3,
                          PchUsbOverCurrentPinSkip,
                          PchUsbOverCurrentPinSkip,
                          PchUsbOverCurrentPinSkip,
                          PchUsbOverCurrentPinSkip
                       };

PCH_USB20_AFE         TypeNeonCityEPECBUsb20AfeParamsLbgB0[PCH_H_XHCI_MAX_USB2_PHYSICAL_PORTS] = {
                        {3, 0, 3, 1},   // PP0
                        {5, 0, 3, 1},   // PP1
                        {3, 0, 3, 1},   // PP2
                        {0, 5, 1, 1},   // PP3
                        {3, 0, 3, 1},   // PP4
                        {3, 0, 3, 1},   // PP5
                        {3, 0, 3, 1},   // PP6
                        {3, 0, 3, 1},   // PP7
                        {2, 2, 1, 0},   // PP8
                        {6, 0, 2, 1},   // PP9
                        {2, 2, 1, 0},   // PP10
                        {6, 0, 2, 1},   // PP11
                        {0, 5, 1, 1},   // PP12
                        {7, 0, 2, 1},   // PP13
                      };

PCH_USB20_AFE         TypeNeonCityEPECBUsb20AfeParams[PCH_H_XHCI_MAX_USB2_PHYSICAL_PORTS] = {
                        {7, 0, 2, 1},   // PP0
                        {7, 0, 2, 1},   // PP1
                        {7, 0, 2, 1},   // PP2
                        {7, 0, 2, 1},   // PP3
                        {7, 0, 2, 1},   // PP4
                        {7, 0, 2, 1},   // PP5
                        {7, 0, 2, 1},   // PP6
                        {7, 0, 2, 1},   // PP7
                        {7, 0, 2, 1},   // PP8
                        {7, 0, 2, 1},   // PP9
                        {7, 0, 2, 1},   // PP10
                        {7, 0, 2, 1},   // PP11
                        {7, 0, 2, 1},   // PP12
                        {7, 0, 2, 1},   // PP13
                      };


EFI_STATUS
TypeNeonCityEPECBPlatformUsbOcUpdateCallback (
  IN OUT   PCH_USB_OVERCURRENT_PIN   **Usb20OverCurrentMappings,
  IN OUT   PCH_USB_OVERCURRENT_PIN   **Usb30OverCurrentMappings,
  IN OUT   PCH_USB20_AFE             **Usb20AfeParams
)
{
  *Usb20OverCurrentMappings   = &TypeNeonCityEPECBUsb20OverCurrentMappings[0];
  *Usb30OverCurrentMappings   = &TypeNeonCityEPECBUsb30OverCurrentMappings[0];

  if (PchStepping() > LbgA0) {
    *Usb20AfeParams   = TypeNeonCityEPECBUsb20AfeParamsLbgB0;
  } else {
    *Usb20AfeParams   = TypeNeonCityEPECBUsb20AfeParams;
  }
  return EFI_SUCCESS;
}

PLATFORM_USBOC_UPDATE_TABLE  TypeNeonCityEPECBUsbOcUpdate =
{
   PLATFORM_USBOC_UPDATE_SIGNATURE,
   PLATFORM_USBOC_UPDATE_VERSION,
   TypeNeonCityEPECBPlatformUsbOcUpdateCallback
};

/**
  The Driver Entry Point.

  The function is the driver Entry point.

  @param ImageHandle   A handle for the image that is initializing this driver
  @param SystemTable   A pointer to the EFI system table

  @retval EFI_SUCCESS:              Driver initialized successfully
  @retval EFI_LOAD_ERROR:           Failed to Initialize or has been loaded
  @retval EFI_OUT_OF_RESOURCES      Could not allocate needed resources

**/
EFI_STATUS
EFIAPI
UsbOcUpdateEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
)
{
  EFI_STATUS                          Status;
  UBA_CONFIG_DATABASE_PROTOCOL        *UbaConfigProtocol = NULL;

  DEBUG((EFI_D_ERROR, "UBA:UsbOcUpdate-TypeNeonCityEPECB\n"));
  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gDxePlatformUbaOcConfigDataGuid,
                                     &TypeNeonCityEPECBUsbOcUpdate,
                                     sizeof(TypeNeonCityEPECBUsbOcUpdate)
                                     );

  return Status;
}
