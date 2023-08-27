// SPDX-FileCopyrightText: 2019-2023 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: (GPL-3.0 OR CC-BY-NC-ND-4.0)

#include "host.h"
#include "fullscreen_ui.h"
#include "gpu.h"
#include "imgui_overlays.h"
#include "shader_cache_version.h"
#include "system.h"

#include "util/gpu_device.h"
#include "util/imgui_manager.h"

#include "common/assert.h"
#include "common/layered_settings_interface.h"
#include "common/log.h"
#include "common/string_util.h"

#include <cstdarg>

Log_SetChannel(Host);

namespace Host {
static std::mutex s_settings_mutex;
static LayeredSettingsInterface s_layered_settings_interface;
} // namespace Host

std::unique_lock<std::mutex> Host::GetSettingsLock()
{
  return std::unique_lock<std::mutex>(s_settings_mutex);
}

SettingsInterface* Host::GetSettingsInterface()
{
  return &s_layered_settings_interface;
}

SettingsInterface* Host::GetSettingsInterfaceForBindings()
{
  SettingsInterface* input_layer = s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_INPUT);
  return input_layer ? input_layer : &s_layered_settings_interface;
}

std::string Host::GetBaseStringSettingValue(const char* section, const char* key, const char* default_value /*= ""*/)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)
    ->GetStringValue(section, key, default_value);
}

bool Host::GetBaseBoolSettingValue(const char* section, const char* key, bool default_value /*= false*/)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)
    ->GetBoolValue(section, key, default_value);
}

s32 Host::GetBaseIntSettingValue(const char* section, const char* key, s32 default_value /*= 0*/)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)
    ->GetIntValue(section, key, default_value);
}

u32 Host::GetBaseUIntSettingValue(const char* section, const char* key, u32 default_value /*= 0*/)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)
    ->GetUIntValue(section, key, default_value);
}

float Host::GetBaseFloatSettingValue(const char* section, const char* key, float default_value /*= 0.0f*/)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)
    ->GetFloatValue(section, key, default_value);
}

double Host::GetBaseDoubleSettingValue(const char* section, const char* key, double default_value /* = 0.0f */)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)
    ->GetDoubleValue(section, key, default_value);
}

std::vector<std::string> Host::GetBaseStringListSetting(const char* section, const char* key)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)->GetStringList(section, key);
}

std::string Host::GetStringSettingValue(const char* section, const char* key, const char* default_value /*= ""*/)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetStringValue(section, key, default_value);
}

bool Host::GetBoolSettingValue(const char* section, const char* key, bool default_value /*= false*/)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetBoolValue(section, key, default_value);
}

s32 Host::GetIntSettingValue(const char* section, const char* key, s32 default_value /*= 0*/)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetIntValue(section, key, default_value);
}

u32 Host::GetUIntSettingValue(const char* section, const char* key, u32 default_value /*= 0*/)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetUIntValue(section, key, default_value);
}

float Host::GetFloatSettingValue(const char* section, const char* key, float default_value /*= 0.0f*/)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetFloatValue(section, key, default_value);
}

double Host::GetDoubleSettingValue(const char* section, const char* key, double default_value /*= 0.0f*/)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetDoubleValue(section, key, default_value);
}

std::vector<std::string> Host::GetStringListSetting(const char* section, const char* key)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetStringList(section, key);
}

void Host::SetBaseBoolSettingValue(const char* section, const char* key, bool value)
{
  std::unique_lock lock(s_settings_mutex);
  s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)->SetBoolValue(section, key, value);
}

void Host::SetBaseIntSettingValue(const char* section, const char* key, int value)
{
  std::unique_lock lock(s_settings_mutex);
  s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)->SetIntValue(section, key, value);
}

void Host::SetBaseFloatSettingValue(const char* section, const char* key, float value)
{
  std::unique_lock lock(s_settings_mutex);
  s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)->SetFloatValue(section, key, value);
}

void Host::SetBaseStringSettingValue(const char* section, const char* key, const char* value)
{
  std::unique_lock lock(s_settings_mutex);
  s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)->SetStringValue(section, key, value);
}

void Host::SetBaseStringListSettingValue(const char* section, const char* key, const std::vector<std::string>& values)
{
  std::unique_lock lock(s_settings_mutex);
  s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)->SetStringList(section, key, values);
}

bool Host::AddValueToBaseStringListSetting(const char* section, const char* key, const char* value)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)
    ->AddToStringList(section, key, value);
}

bool Host::RemoveValueFromBaseStringListSetting(const char* section, const char* key, const char* value)
{
  std::unique_lock lock(s_settings_mutex);
  return s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)
    ->RemoveFromStringList(section, key, value);
}

void Host::DeleteBaseSettingValue(const char* section, const char* key)
{
  std::unique_lock lock(s_settings_mutex);
  s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE)->DeleteValue(section, key);
}

SettingsInterface* Host::Internal::GetBaseSettingsLayer()
{
  return s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE);
}

SettingsInterface* Host::Internal::GetGameSettingsLayer()
{
  return s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_GAME);
}

SettingsInterface* Host::Internal::GetInputSettingsLayer()
{
  return s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_INPUT);
}

void Host::Internal::SetBaseSettingsLayer(SettingsInterface* sif)
{
  AssertMsg(s_layered_settings_interface.GetLayer(LayeredSettingsInterface::LAYER_BASE) == nullptr,
            "Base layer has not been set");
  s_layered_settings_interface.SetLayer(LayeredSettingsInterface::LAYER_BASE, sif);
}

void Host::Internal::SetGameSettingsLayer(SettingsInterface* sif)
{
  std::unique_lock lock(s_settings_mutex);
  s_layered_settings_interface.SetLayer(LayeredSettingsInterface::LAYER_GAME, sif);
}

void Host::Internal::SetInputSettingsLayer(SettingsInterface* sif)
{
  std::unique_lock lock(s_settings_mutex);
  s_layered_settings_interface.SetLayer(LayeredSettingsInterface::LAYER_INPUT, sif);
}

void Host::ReportFormattedDebuggerMessage(const char* format, ...)
{
  std::va_list ap;
  va_start(ap, format);
  std::string message = StringUtil::StdStringFromFormatV(format, ap);
  va_end(ap);

  ReportDebuggerMessage(message);
}

bool Host::CreateGPUDevice(RenderAPI api)
{
  DebugAssert(!g_gpu_device);

  Log_InfoPrintf("Trying to create a %s GPU device...", GPUDevice::RenderAPIToString(api));
  g_gpu_device = GPUDevice::CreateDeviceForAPI(api);

  // TODO: FSUI should always use vsync..
  const bool vsync = System::IsValid() ? System::ShouldUseVSync() : g_settings.video_sync_enabled;
  if (!g_gpu_device || !g_gpu_device->Create(g_settings.gpu_adapter,
                                             g_settings.gpu_disable_shader_cache ? std::string_view() :
                                                                                   std::string_view(EmuFolders::Cache),
                                             SHADER_CACHE_VERSION, g_settings.gpu_use_debug_device, vsync,
                                             g_settings.gpu_threaded_presentation))
  {
    Log_ErrorPrintf("Failed to initialize GPU device.");
    if (g_gpu_device)
      g_gpu_device->Destroy();
    g_gpu_device.reset();
    return false;
  }

  if (!ImGuiManager::Initialize(g_settings.display_osd_scale / 100.0f))
  {
    Log_ErrorPrintf("Failed to initialize ImGuiManager.");
    g_gpu_device->Destroy();
    g_gpu_device.reset();
    return false;
  }

  return true;
}

void Host::UpdateDisplayWindow()
{
  if (!g_gpu_device)
    return;

  if (!g_gpu_device->UpdateWindow())
  {
    Host::ReportErrorAsync("Error", "Failed to change window after update. The log may contain more information.");
    return;
  }

  ImGuiManager::WindowResized();

  // If we're paused, re-present the current frame at the new window size.
  if (System::IsValid() && System::IsPaused())
    RenderDisplay(false);
}

void Host::ResizeDisplayWindow(s32 width, s32 height, float scale)
{
  if (!g_gpu_device)
    return;

  Log_DevPrintf("Display window resized to %dx%d", width, height);

  g_gpu_device->ResizeWindow(width, height, scale);
  ImGuiManager::WindowResized();

  // If we're paused, re-present the current frame at the new window size.
  if (System::IsValid())
  {
    if (System::IsPaused())
      RenderDisplay(false);

    System::HostDisplayResized();
  }
}

void Host::ReleaseGPUDevice()
{
  if (!g_gpu_device)
    return;

  SaveStateSelectorUI::DestroyTextures();
  FullscreenUI::Shutdown();
  ImGuiManager::Shutdown();

  Log_InfoPrintf("Destroying %s GPU device...", GPUDevice::RenderAPIToString(g_gpu_device->GetRenderAPI()));
  g_gpu_device->Destroy();
  g_gpu_device.reset();
}

#ifndef __ANDROID__

std::unique_ptr<AudioStream> Host::CreateAudioStream(AudioBackend backend, u32 sample_rate, u32 channels, u32 buffer_ms,
                                                     u32 latency_ms, AudioStretchMode stretch)
{
  switch (backend)
  {
#ifdef WITH_CUBEB
    case AudioBackend::Cubeb:
      return AudioStream::CreateCubebAudioStream(sample_rate, channels, buffer_ms, latency_ms, stretch);
#endif

#ifdef _WIN32
    case AudioBackend::XAudio2:
      return AudioStream::CreateXAudio2Stream(sample_rate, channels, buffer_ms, latency_ms, stretch);
#endif

    case AudioBackend::Null:
      return AudioStream::CreateNullStream(sample_rate, channels, buffer_ms);

    default:
      return nullptr;
  }
}

#endif

void Host::RenderDisplay(bool skip_present)
{
  Host::BeginPresentFrame();

  // acquire for IO.MousePos.
  std::atomic_thread_fence(std::memory_order_acquire);

  if (!skip_present)
  {
    FullscreenUI::Render();
    ImGuiManager::RenderTextOverlays();
    ImGuiManager::RenderOSDMessages();
  }

  // Debug windows are always rendered, otherwise mouse input breaks on skip.
  ImGuiManager::RenderOverlayWindows();
  ImGuiManager::RenderDebugWindows();

  g_gpu_device->Render(skip_present);

  ImGuiManager::NewFrame();

  if (g_gpu)
    g_gpu->RestoreGraphicsAPIState();
}

void Host::InvalidateDisplay()
{
  RenderDisplay(false);
}
