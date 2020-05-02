#ifndef ENSEMBLEAPI_H
#define ENSEMBLEAPI_H

#include "ErrorType.h"
#include "EnsembleCommon.h"
#include <string>
#include "InterfaceControl.h"

class CEnsembleAPI
{

public:
	CEnsembleAPI(const std::string str_ip="", const int port=4000) ;
	~CEnsembleAPI(void);

private:
	CInterfaceControl m_cls_interface_control;
	
	std::string m_str_ip_addr ;
	unsigned int m_port ;
	
public:
	//[0] Connect
	int Ensemble_Network_Connect(const char* ip, const int port);
	void Ensemble_Network_Disconnect(void);
	int Ensemble_Network_IsOnline(void);
	int Ensemble_Network_GetInfo(std::string* out_str_ip, int* out_port);

	//[1]Source
	std::string Ensemble_Source_Get_List(void);
	int Ensemble_Source_Set(const std::string source);
	std::string Ensemble_DB_Get_List(void) ;


	//[2] Get Image Stream
	int Ensemble_Source_Get_Image(const int type_option, const int width, const int height, ImageBuf* p_buf) ;
	int Ensemble_Source_Get_Image(const int type_option, ImageBuf* p_buf) ;
	int Ensemble_Source_Get_CalibrationImage(const std::string id, const int type_option, const int width, const int height, ImageBuf* p_buf) ;
	int Ensemble_Source_Get_CalibrationImage(const std::string id, const int type_option, ImageBuf* p_buf) ;
	int Ensemble_DeviceIcon_Get_Image(ImageBuf* p_buf) ;
	int Ensemble_Result_Get_Image(const std::string id, const int type_option, const int width, const int height, ImageBuf* p_buf) ;
	int Ensemble_Result_Get_Image(const std::string id, const int type_option, ImageBuf* p_buf) ;


	//[3] Job Management
	std::string Ensemble_Task_Get_Parent_Tree(const std::string id) ;
	int Ensemble_Task_Get_Run_Option(const std::string id) ;
	int Ensemble_Task_Get_View_Option(const std::string id) ;
	int Ensemble_Task_Set_Run_Option(const std::string id, const bool run) ;
	int Ensemble_Task_Set_View_Option(const std::string id, const bool view) ;
	int Ensemble_Task_File_Save(const bool b_overwrite=false) ;		//save all
	int Ensemble_Task_File_Load(void) ;	//load all
    int Ensemble_Task_File_Load(std::vector<std::string> vec_str_db_id) ;	//load db
	int Ensemble_Task_Clear(void) ;	//load db

	//Information
	std::string Ensemble_Info_Type_Get_Tool_List_Xml(void) ;
	std::string Ensemble_Info_Get_ToolTypeName(const int type) ;
	std::string Ensemble_Info_Type_Get_Job_List_Xml(void) ;
	int Ensemble_Info_Get_Type(const std::string id) ;


	//[3-1] Project
	int Ensemble_Project_Add_New(const std::string name="") ;
	int Ensemble_Project_Del(const std::string proj_id="") ;
	std::string Ensemble_Project_Get_List(void) ;
	std::string Ensemble_Project_Get_Name(const std::string project_id) ;
	std::string Ensemble_Project_Get_Has_Job_Info(const std::string project_id) ;
	int Ensemble_Project_Set_Name(const std::string project_id, const std::string name) ;
	//Run
	int Ensemble_Poject_Set_Trigger_Run(const std::string id, const bool b_set) ;
	int Ensemble_Poject_Get_Trigger_Run(const std::string id) ;

	std::string Ensemble_Poject_Run(const std::string id) ;
	int Ensemble_Project_Get_ResultImage(const std::string id, const int type_option, const int width, const int height, ImageBuf* p_buf) ;
	int Ensemble_Project_Get_ResultImage(const std::string id, const int type_option, ImageBuf* p_buf) ;


	//[3-2] Job
	int Ensemble_Job_Add_New(const std::string project_id, const int type, const std::string name="") ;
	std::string Ensemble_Job_Get_TypeName(const int job_type) ;
	int Ensemble_Job_Del(const std::string id) ;
	std::string Ensemble_Job_Get_Name(const std::string id) ;
	int Ensemble_Job_Set_Name(const std::string id, const std::string name) ;
	int Ensemble_Job_Get_Image(const std::string id,const int type_option,  const int width, const int height, ImageBuf* p_buf) ;
	int Ensemble_Job_Get_Image(const std::string id, const int type_option, ImageBuf* p_buf) ;
	int Ensemble_Job_Get_ObjectImage(const std::string id,const int type_option,  const int width, const int height, ImageBuf* p_buf) ;
	int Ensemble_Job_Get_ObjectImage(const std::string id, const int type_option, ImageBuf* p_buf) ;
	int Ensemble_Job_Set_Image(const std::string id) ;
	int Ensemble_Job_Set_DetectArea(const std::string id, const float x, const float y, const float width, const float height) ;
	int Ensemble_Job_Set_Zoom(const std::string id, const float x, const float y, const float width, const float height) ;
	int Ensemble_Job_Set_MaskArea(const std::string id, float x, float y, float w, float h, bool inverse);
	int Ensemble_Job_Undo_MaskArea(const std::string id);
	int Ensemble_Job_Del_MaskArea(const std::string id);
	int Ensemble_Job_Get_FeatureLevel(const std::string id);
	int Ensemble_Job_Set_FeatureLevel(const std::string id, const int level);
	int Ensemble_Job_Get_UseCustomFeatureOption(const std::string id);
	int Ensemble_Job_Set_UseCustomFeatureOption(const std::string id, const bool b_use);
	int Ensemble_Job_Get_Feature_Option(const std::string id, int* out_param1, int* out_param2, int* out_param3, int* out_param4);
	int Ensemble_Job_Set_Feature_Option(const std::string id, const int param1, const int param2, const int param3, const int param4);
	int Ensemble_Job_Set_SelectObject(const std::string id, const float x, const float y, const float width, const float height) ;
	int Ensemble_Job_Del_SelectObject(const std::string id) ;
	int Ensemble_Job_Set_Erase(const std::string id, const float x, const float y, const float width, const float height) ;
	int Ensemble_Job_Del_Erase(const std::string id) ;
	int Ensemble_Job_Set_Ref_Point(const std::string id, const float x, const float y) ;
	int Ensemble_Job_Del_Ref_Point(const std::string id) ;
	int Ensemble_Job_Set_DetectOption(const std::string id, const int option, const float value) ;
	float Ensemble_Job_Get_DetectOption(const std::string id, const int option) ;
	int Ensemble_Job_Get_ResultImage(const std::string id, const int type_option, const int width, const int height, ImageBuf* p_buf) ;
	int Ensemble_Job_Get_ResultImage(const std::string id, const int type_option, ImageBuf* p_buf) ;
	int Ensemble_Job_Set_Python_Code(const std::string id, const std::string code) ;
	std::string Ensemble_Job_Get_Python_Code(const std::string id) ;
	int Ensemble_Job_Run_Python_Code(const std::string id) ;
	//Run
	std::string Ensemble_Job_Run(const std::string id, const float masking_left_top_x=-1, const float masking_left_top_y=-1, const float masking_right_top_x=-1, const float masking_right_top_y=-1, const float masking_right_bottom_x=-1, const float masking_right_bottom_y=-1, const float masking_left_bottom_x=-1, const float masking_left_bottom_y=-1) ;

	//[3-3] Tool
	int Ensemble_Tool_Add_New(const std::string parent_id, const int tool_type) ;
	int Ensemble_Tool_Insert(const std::string parent_id, const int index, const int tool_type) ;
	int Ensemble_Tool_Move(const std::string parent_id, const int from_index, const int target_index) ;
	int Ensemble_Tool_Del(const std::string tool_id) ;
	std::string Ensemble_Tool_Get_Name(const std::string tool_id) ;
	int Ensemble_Tool_Set_Name(const std::string tool_id, const std::string name) ;
	int Ensemble_Tool_Get_ObjectImage(const std::string tool_id,const int type_option,  const int width, const int height, ImageBuf* p_buf) ;
	int Ensemble_Tool_Get_ObjectImage(const std::string tool_id, const int type_option, ImageBuf* p_buf) ;
	int Ensemble_Tool_Get_Image(const std::string tool_id, const int type_option,  const int width, const int height, ImageBuf* p_buf) ;
	int Ensemble_Tool_Get_Image(const std::string tool_id, const int type_option, ImageBuf* p_buf) ;

	//[3-3] Tool : Detect Object, Line, Circle
	int Ensemble_Tool_Set_MaskArea(const std::string tool_id, float x, float y, float w, float h, bool inverse);
	int Ensemble_Tool_Undo_MaskArea(const std::string tool_id);
	int Ensemble_Tool_Del_MaskArea(const std::string tool_id);
	int Ensemble_Tool_Get_FeatureLevel(const std::string tool_id);
	int Ensemble_Tool_Set_FeatureLevel(const std::string tool_id, const int level);
	int Ensemble_Tool_Get_UseCustomFeatureOption(const std::string id);
	int Ensemble_Tool_Set_UseCustomFeatureOption(const std::string id, const bool b_use);
	int Ensemble_Tool_Get_Feature_Option(const std::string id, int* out_param1, int* out_param2, int* out_param3, int* out_param4);
	int Ensemble_Tool_Set_Feature_Option(const std::string id, const int param1, const int param2, const int param3, const int param4);
	int Ensemble_Tool_Set_SelectObject(const std::string tool_id, const float x, const float y, const float width, const float height, const int margin=0) ;
	int Ensemble_Tool_Set_SelectObject(const std::string tool_id, const float left_top_x, const float left_top_y, const float right_top_x, const float right_top_y, const float right_bottom_x, const float right_bottom_y, const float left_bottom_x, const float left_bottom_y, const int margin=0) ;
	int Ensemble_Tool_Set_Ref_Point(const std::string id, const float x, const float y) ;
	int Ensemble_Tool_Del_Ref_Point(const std::string id) ;
	int Ensemble_Tool_Set_DetectOption(const std::string tool_id,const int option, const float value) ;
	float Ensemble_Tool_Get_DetectOption(const std::string tool_id,const int option) ;

	//[3-3] Tool : Detect Code
	std::string Ensemble_Tool_Detect_Code_Get_Ref_CodeType(const std::string tool_id) ;
	std::string Ensemble_Tool_Detect_Code_Get_Ref_CodeData(const std::string tool_id) ;
	float Ensemble_Tool_Detect_Code_Get_Padding(const std::string tool_id) ;
	int Ensemble_Tool_Detect_Code_Set_Padding(const std::string tool_id, const float padding_rate) ;


	//[3-3] Tool : Detect Object
	int Ensemble_Tool_Detect_Object_Get_Inspection_Tolerance_Info(const std::string tool_id, float* out_score_threshold) ;
	int Ensemble_Tool_Detect_Object_Set_Inspection_Tolerance_Info(const std::string tool_id, const float score_threshold) ;

	//[3-3] Tool : Detect Line
	int Ensemble_Tool_Detect_Line_Set_SelectObject(const std::string tool_id, const float line1_x, const float line1_y, const float line2_x, const float line2_y, const float left_top_x, const float left_top_y, const float right_top_x, const float right_top_y, const float right_bottom_x, const float right_bottom_y, const float left_bottom_x, const float left_bottom_y) ;

	//[3-3] Tool : Offset Distance
	int Ensemble_Tool_Offset_Distance_Set_Direction(const std::string tool_id, const int direction) ;
	int Ensemble_Tool_Offset_Distance_Get_Direction(const std::string tool_id) ;
	int Ensemble_Tool_Offset_Distance_Set_Region(const std::string tool_id, const float x, const float y, const float width, const float height) ;
	int Ensemble_Tool_Offset_Distance_Get_Region(const std::string tool_id, float* out_x, float* out_y, float* out_width, float* out_height) ;
	int Ensemble_Tool_Offset_Distance_Get_Inspection_Base_Info(const std::string tool_id, float* out_distance, float *out_angle) ;
	int Ensemble_Tool_Offset_Distance_Get_Inspection_Tolerance_Info(const std::string tool_id, float* out_distance_min, float *out_distance_max, float* out_angle_min, float *out_angle_max) ;
	int Ensemble_Tool_Offset_Distance_Set_Inspection_Tolerance_Info(const std::string tool_id, const float distance_min, const float distance_max, const float angle_min, const float angle_max) ;



	//[3-3] Tool : Calc Distance
	float Ensemble_Tool_Distance_Get_CalcDistance(const std::string tool_id) ;
	int Ensemble_Tool_Distance_Set_BaseDistance(const std::string tool_id, const float base_distance) ;
	float Ensemble_Tool_Distance_Get_BaseDistance(const std::string tool_id) ;
	int Ensemble_Tool_Option_InspectDistance_Get_Tolerance(const std::string option_id, float* out_min, float* out_max) ;
	int Ensemble_Tool_Option_InspectDistance_Set_Tolerance(const std::string option_id, const float min, const float max) ;
	std::string Ensemble_Tool_Option_InspectDistance_Get_ID_Info_Base(const std::string option_id) ;
	std::string Ensemble_Tool_Option_InspectDistance_Get_ID_Info_Target(const std::string option_id) ;

	//[3-4] Option
    int Ensemble_Tool_Option_GetImage(const std::string option_id, const int type_option,  const int width, const int height, ImageBuf* p_buf) ;
    int Ensemble_Tool_Option_GetImage(const std::string option_id, const int type_option, ImageBuf* p_buf) ;
	std::string Ensemble_Tool_Get_OptionList(const std::string tool_id) ;
	std::string Ensemble_Tool_Get_OptionList(const int type) ;
	int Ensemble_Tool_Get_OptionList_Count(const int type) ;
	int Ensemble_Tool_Add_NewOption(const std::string tool_id, const int option_type) ;
	int Ensemble_Tool_Del_Option(const std::string option_id) ;

	//[3-4] Option Inspect Color
    int Ensemble_Tool_Option_InspectColor_Histogram_GetImage(const std::string option_id, const int color_num, const int type_option,  const int width, const int height, ImageBuf* p_buf) ;
    int Ensemble_Tool_Option_InspectColor_Histogram_GetImage(const std::string option_id, const int color_num, const int type_option, ImageBuf* p_buf) ;
	int Ensemble_Tool_Option_InspectColor_Set_Histogram_UseElement(const std::string option_id, const int color_elem) ;
	int Ensemble_Tool_Option_InspectColor_Get_Histogram_UseElement(const std::string option_id) ;
	int Ensemble_Tool_Option_InspectColor_Set_Histogram_Range(const std::string option_id, const int color_elem, const float min, const float max) ;
	int Ensemble_Tool_Option_InspectColor_Get_Histogram_Range(const std::string option_id, const int color_elem, int* out_min, int* out_max) ;
	int Ensemble_Tool_Option_InspectColor_Get_Base_Pixel_Count(const std::string option_id) ;
	int Ensemble_Tool_Option_InspectColor_Get_Tolerance(const std::string option_id, int* out_min, int* out_max) ;
	int Ensemble_Tool_Option_InspectColor_Set_Tolerance(const std::string option_id, const int min, const int max) ;

	//[3-4] Option Inspect Angle
	float Ensemble_Tool_Angle_Get_CalcAngle(const std::string tool_id) ;
	int Ensemble_Tool_Option_InspectAngle_Get_Tolerance(const std::string option_id, float* out_min, float* out_max) ;
	int Ensemble_Tool_Option_InspectAngle_Set_Tolerance(const std::string option_id, const float min, const float max) ;
	std::string Ensemble_Tool_Option_InspectAngle_Get_ID_Info_Base(const std::string option_id) ;
	std::string Ensemble_Tool_Option_InspectAngle_Get_ID_Info_Target(const std::string option_id) ;

	//[3-4] Option Inspect Diameter
	float Ensemble_Tool_Circle_Get_CalcDiameter(const std::string tool_id) ;
	int Ensemble_Tool_Option_InspectDiameter_Get_Tolerance(const std::string option_id, float* out_min, float* out_max) ;
	int Ensemble_Tool_Option_InspectDiameter_Set_Tolerance(const std::string option_id, const float min, const float max) ;

	////[3-4] Option Inspect Crack
	int Ensemble_Tool_Option_Crack_Get_InspectLevel(const std::string option_id);
	int Ensemble_Tool_Option_Crack_Set_InspectLevel(const std::string option_id, const int level);
	
	////[3-4] Option Inspect Color Compare
	int Ensemble_Tool_Option_ColorCompare_Get_Sensitivity(const std::string option_id);
	int Ensemble_Tool_Option_ColorCompare_Set_Sensitivity(const std::string option_id, const int level);
	int Ensemble_Tool_Option_ColorCompare_Get_InspectLevel(const std::string option_id);
	int Ensemble_Tool_Option_ColorCompare_Set_InspectLevel(const std::string option_id, const int level);

	//[5] Calibration
	int Ensemble_Job_Calibration_Get_Chess_Info(const std::string job_id, int *out_x_num, int *out_y_num, float *out_squre_mm_size);
	int Ensemble_Job_Calibration_Set_Chess_Info(const std::string job_id, const int x_num, const int y_num, const float squre_mm_size);
	int Ensemble_Job_Calibration_Add(const std::string job_id, float robot_x, float robot_y);
	int Ensemble_Job_Calibration_GetCount(const std::string job_id);
	int Ensemble_Job_Calibration_GetImage(const std::string job_id, const int index, const int type_option, const int width, const int height, ImageBuf* p_buf) ;
	int Ensemble_Job_Calibration_GetImage(const std::string job_id, const int index, const int type_option, ImageBuf* p_buf) ;
	int Ensemble_Job_Calibration_GetRobotInfo(const std::string job_id, int index, float *out_robot_x, float *out_robot_y);
	int Ensemble_Job_Calibration_Del(const std::string job_id, int index);
	int Ensemble_Job_Calibration_Clear(const std::string job_id);
	int Ensemble_Job_Calibration_Run(const std::string job_id);
	int Ensemble_Job_Calibration_StandAlone_Init(const std::string job_id);
	int Ensemble_Job_Calibration_StandAlone_Run(const std::string job_id);
	int Ensemble_Job_Calibration_StandAlone_Get_Image_Count(const std::string job_id);
	int Ensemble_Job_Calibration_StandAlone_Get_Feature_Pos(const std::string job_id, int index, float posA[3], float posB[3], float posC[3], float posD[3]);
	int Ensemble_Job_Calibration_StandAlone_Set_Matrix(const std::string job_id, float matrix[12]);
	int Ensemble_Job_Calibration_StandAlone_Get_Matrix(const std::string job_id, float matrix[12]);
	int Ensemble_Job_Calibration_StandAlone_Calc_Calib_Matrix(const std::string job_id, 
																				float cposA[3], float cposB[3], float cposC[3], float cposD[3],
																				float rposA[3], float rposB[3], float rposC[3], float rposD[3],
																				float ret_Matrix[12]);
	int Ensemble_Job_Calibration_StandAlone_Y_Direction(const std::string job_id, int Direction);
	int Ensemble_Job_Calibration_Custom_Center_Point(const std::string job_id, int customnum);
	int Ensemble_Job_Calibration_GetPoint(const std::string job_id, const float in_px, const float in_py, float* out_rx, float* out_ry);
	int Ensemble_Job_Calibration_GetChessPoint(const std::string job_id, const int index, float* out_rx, float* out_ry);
	int Ensemble_Job_Calibration_isOK(const std::string job_id);
	int Ensemble_Job_Calibration_Copy(const std::string job_id, const std::string from_job_id);

	//[6] Camera Tool
	int Ensemble_Camera_Set_Ready(const std::string job_id);
	int Ensemble_Camera_Set_Auto_Exposure(const std::string job_id);
	int Ensemble_Camera_Set_Manual_Exposure_Value(const std::string job_id, const int value);
	int Ensemble_Camera_Get_Manual_Exposure_Value(const std::string job_id);
	int Ensemble_Camera_Get_Min_Exposure_Value(const std::string job_id);
	int Ensemble_Camera_Get_Max_Exposure_Value(const std::string job_id);
	int Ensemble_Camera_Set_Manual_Gain_Value(const std::string job_id, const int value);
	int Ensemble_Camera_Get_Manual_Gain_Value(const std::string job_id);
	int Ensemble_Camera_Get_Min_Gain_Value(const std::string job_id);
	int Ensemble_Camera_Get_Max_Gain_Value(const std::string job_id);
	int Ensemble_Camera_Set_Auto_Focus(const std::string job_id, const float x=-1, const float y=-1, const float width=-1, const float height=-1);
	int Ensemble_Camera_Get_Auto_Focus_Area(const std::string job_id, float *out_x, float *out_y, float *out_width, float *out_height);
	int Ensemble_Camera_Set_Manual_Focus_Value(const std::string job_id, const int value);
	int Ensemble_Camera_Get_Manual_Focus_Value(const std::string job_id);
	int Ensemble_Camera_Get_Min_Focus_Value(const std::string job_id);
	int Ensemble_Camera_Get_Max_Focus_Value(const std::string job_id);
	int Ensemble_Camera_Set_Manual_Brightness_Value(const std::string job_id, const int value);
	int Ensemble_Camera_Get_Manual_Brightness_Value(const std::string job_id);
	int Ensemble_Camera_Get_Min_Brightness_Value(const std::string job_id);
	int Ensemble_Camera_Get_Max_Brightness_Value(const std::string job_id);
	int Ensemble_Camera_Set_Manual_Sharpness_Value(const std::string job_id, const int value);
	int Ensemble_Camera_Get_Manual_Sharpness_Value(const std::string job_id);
	int Ensemble_Camera_Get_Min_Sharpness_Value(const std::string job_id);
	int Ensemble_Camera_Get_Max_Sharpness_Value(const std::string job_id);
	int Ensemble_Camera_Set_Manual_Contrast_Value(const std::string job_id, const int value);
	int Ensemble_Camera_Get_Manual_Contrast_Value(const std::string job_id);
	int Ensemble_Camera_Get_Min_Contrast_Value(const std::string job_id);
	int Ensemble_Camera_Get_Max_Contrast_Value(const std::string job_id);
	int Ensemble_Camera_Set_Manual_ISO_Value(const std::string job_id, const int value);
	int Ensemble_Camera_Get_Manual_ISO_Value(const std::string job_id);
	int Ensemble_Camera_Get_Min_ISO_Value(const std::string job_id);
	int Ensemble_Camera_Get_Max_ISO_Value(const std::string job_id);
	int Ensemble_Camera_Set_Manual_Shutter_Speed_Value(const std::string job_id, const int value);
	int Ensemble_Camera_Get_Manual_Shutter_Speed_Value(const std::string job_id);
	int Ensemble_Camera_Get_Min_Shutter_Speed_Value(const std::string job_id);
	int Ensemble_Camera_Get_Max_Shutter_Speed_Value(const std::string job_id);
	int Ensemble_Camera_Set_Flip_V(const std::string job_id, const bool flip);
	int Ensemble_Camera_Get_Flip_V(const std::string job_id);
	int Ensemble_Camera_Set_Flip_H(const std::string job_id, const bool flip);
	int Ensemble_Camera_Get_Flip_H(const std::string job_id);
	int Ensemble_Camera_Set_Factory_Reset(const std::string job_id);
	int Ensemble_Camera_Set_Config_Run(const std::string job_id);

	int Ensemble_Camera_Capture_SW_Trigger(void) ;
	int Ensemble_Camera_Capture_HW_Trigger(void) ;

	int Ensemble_Camera_Get_Frame_Count();	
	int Ensemble_Camera_Set_Camera_Image_To_Past_Frame(int FrameNum);

    int Ensemble_Camera_Save_Image_To_Device_Local(const std::string file_name);
	
	int Ensemble_Digital_IO_GetIn(void) ;
	int Ensemble_Digital_IO_SetOut(int pin_num, int pin_val) ;

	//[7] Ensemble binary Update Tool
	std::string Ensemble_Update_Get_Version_List(void);
	std::string Ensemble_Update_Get_Current_Version(void);
	int Ensemble_Update_Set_Version(std::string version_string);
};

#endif
