#ifndef INTERFACE_CONTROL_H
#define INTERFACE_CONTROL_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>

#include "EnsembleCommand.h"
#include "Interface.h"
#include "EthernetClient.h"
#include "EthernetClientControlData.h"

//#include <qdebug.h>

#define TRANS_IAMGE_WIDTH		640
#define TRANS_IAMGE_HEIGHT		480

class  CInterfaceControl : public CInterface
{
public:
	CInterfaceControl(void);
	~CInterfaceControl(void);

    int IsOnline(void);

    int SetNetworkIp(const std::string str_ip) ;
	
	//-----------------------------
	//Image and Source
    int GetImage(const int option, std::string id, const int type_option, int& width, int& height, ImageBuf* out_buf);
	std::string GetSourceList(void) ;
	int SetSource(const std::string source) ;
	std::string DB_Get_List(void) ;
	int GetResultImage(const std::string id, const int type_option, int& width, int& height, ImageBuf* out_buf);
	//Image and Source
	//-----------------------------

	std::string Task_Get_Parent_Tree(const std::string id) ;
	int Get_Run_Option(const std::string id) ;
	int Get_View_Option(const std::string id) ;
	int Base_Set_Run_Option(const std::string id, const bool run) ;
	int Base_Set_View_Option(const std::string id, const bool view) ;
	int Task_Save(const bool b_overwrite) ;
	int Task_Load(void) ;
	int Task_Load(std::vector<std::string>   vec_str_db_id) ;
	int Task_Clear(void) ;

	int Project_Add_New(const std::string name) ;
	int Project_Del(const std::string id) ;
	std::string Project_Get_List(void) ;
	std::string Project_Get_Name(const std::string id) ;
	std::string Project_Get_Job_Info(const std::string id) ;
	int Project_Set_Name(const std::string id, const std::string name) ;
	int Poject_Set_Trigger_Run(const std::string id, const bool b_set) ;
	int Poject_Get_Trigger_Run(const std::string id) ;
	std::string Project_Run(const std::string id) ;
	int Project_GetResultImage(const std::string id, const int type_option, int& width, int& height, ImageBuf* out_buf);

	std::string Job_Type_Get_List_Xml(void) ;
		
	int Job_Add_New(const std::string project_id, const int type, const std::string name) ;
	std::string Job_Get_TypeName(const int job_type);
	
	int DelJob(const std::string id) ;
	std::string JobGetName(const std::string id) ;
	int JobChangeName(const std::string id, const std::string name) ;
	int JobGetImage(const std::string id, const int type_option, int& width, int& height, ImageBuf* out_buf) ;
	int JobGetObjectImage(const std::string id, const int type_option, int& width, int& height, ImageBuf* out_buf) ;
	int Job_GetResultImage(const std::string id, const int type_option, int& width, int& height, ImageBuf* out_buf);
	int JobSetImage(const std::string id) ;
	int JobSetDetectArea(const std::string id, const float x, const float y, const float width, const float height) ;
	int JobSetZoom(const std::string id, const float x, const float y, const float width, const float height) ;
	int JobSelectObject(const std::string id, const float x, const float y, const float width, const float height) ;
	int JobSetDetectOption(const std::string id, const int option, const float value) ;
	int JobSetRefPoint(const std::string id, const float x, const float y) ;
	int JobDelRefPoint(const std::string id) ;
	float JobGetDetectOption(const std::string id, const int option) ;
	int JobResetObject(const std::string id) ;	
	int Job_Set_Erase(const std::string id, const float x, const float y, const float width, const float height) ;
	int Job_Del_Erase(const std::string id) ;
	int JobSetMaskArea(const std::string id, float x, float y, float w, float h, bool inverse);
	int JobUndoMaskArea(const std::string id);
	int JobDelMaskArea(const std::string id);
	int JobGetFeatureLevel(const std::string id);
	int JobSetFeatureLevel(const std::string id, const int level);
	int JobGetUseCustomFeatureOption(const std::string id);
	int JobSetUseCustomFeatureOption(const std::string id, const bool b_use);
	int Job_Get_Feature_Option(const std::string id, int* out_param1, int* out_param2, int* out_param3, int* out_param4);
	int Job_Set_Feature_Option(const std::string id, const int param1, const int param2, const int param3, const int param4);
	int Job_Set_Python_Code(const std::string id, const std::string code) ;
	std::string Job_Get_Python_Code(const std::string id) ;
	int Job_Run_Python_Code(const std::string id) ;
	
	std::string JobRun(const std::string id, const float masking_left_top_x, const float masking_left_top_y, const float masking_right_top_x, const float masking_right_top_y, const float masking_right_bottom_x, const float masking_right_bottom_y, const float masking_left_bottom_x, const float masking_left_bottom_y);

	//Calibration
    int Calibration_Get_Chess_Info(const std::string job_id, int *out_x_num, int *out_y_num, float *out_squre_mm_size) ;
    int Calibration_Set_Chess_Info(const std::string job_id, const int x_num, const int y_num, const float squre_mm_size) ;
	int Calibration_Add(const std::string job_id, float robot_x, float robot_y) ;
	int Calibration_GetCount(const std::string job_id) ;
	int Calibration_GetImage(const std::string job_id, int index, const int type_option, int& width, int& height, ImageBuf* out_buf) ;
	int Calibration_GetRobotInfo(const std::string job_id, int index, float *out_robot_x, float *out_robot_y);
	int Calibration_Del(const std::string job_id, int index);
	int Calibration_Clear(const std::string job_id);
	int Calibration_Run(const std::string job_id);
	int Calibration_Custom_Center_Point(const std::string job_id, int customnum); 
	int Calibration_GetPoint(const std::string job_id, const float in_px, const float in_py, float* out_rx, float* out_ry);
	int Calibration_GetChessPoint(const std::string job_id, const int index, float* out_rx, float* out_ry);
	int Calibration_isOK(const std::string job_id);
	int Calibration_Copy(const std::string job_id, const std::string from_job_id) ;
	int Calibration_StandAlone_Run(const std::string job_id);
	int Calibration_StandAlone_Get_Image_Count(const std::string job_id);
	int Calibration_StandAlone_Init(const std::string job_id);
	int Calibration_StandAlone_Get_Feature_Pos(const std::string job_id, int index,float posA[3], float posB[3], float posC[3], float posD[3]);
	int Calibration_StandAlone_Set_Matrix(const std::string job_id, float matrix[12]);
	int Calibration_StandAlone_Get_Matrix(const std::string job_id, float matrix[12]);
	int Calibration_StandAlone_Calc_Calib_Matrix(const std::string job_id, 
															float cposA[3], float cposB[3], float cposC[3], float cposD[3],
															float rposA[3], float rposB[3], float rposC[3], float rposD[3],
															float ret_Matrix[12]);
	int Calibration_StandAlone_Y_Direction(const std::string job_id, int Direction);

	//Camera
	int Camera_Set_Ready(const std::string job_id) ;
	int Camera_Set_Auto_Exposure(const std::string job_id);
	int Camera_Set_Manual_Exposure_Value(const std::string job_id, const int value);
	int Camera_Get_Manual_Exposure_Value(const std::string job_id);
	int Camera_Get_Min_Exposure_Value(const std::string job_id);
	int Camera_Get_Max_Exposure_Value(const std::string job_id);
	int Camera_Set_Manual_Gain_Value(const std::string job_id, const int value);
	int Camera_Get_Manual_Gain_Value(const std::string job_id);
	int Camera_Get_Min_Gain_Value(const std::string job_id);
	int Camera_Get_Max_Gain_Value(const std::string job_id);
	int Camera_Set_Auto_Focus(const std::string job_id, const float x, const float y, const float width, const float height);
	int Camera_Get_Auto_Focus_Area(const std::string job_id, float *out_x, float *out_y, float *out_width, float *out_height) ;
	int Camera_Set_Manual_Focus_Value(const std::string job_id, const int value);
	int Camera_Get_Manual_Focus_Value(const std::string job_id);
	int Camera_Get_Min_Focus_Value(const std::string job_id);
	int Camera_Get_Max_Focus_Value(const std::string job_id);
	int Camera_Set_Manual_Brightness_Value(const std::string job_id, const int value);
	int Camera_Get_Manual_Brightness_Value(const std::string job_id);
	int Camera_Get_Min_Brightness_Value(const std::string job_id);
	int Camera_Get_Max_Brightness_Value(const std::string job_id);
	int Camera_Set_Manual_Sharpness_Value(const std::string job_id, const int value);
	int Camera_Get_Manual_Sharpness_Value(const std::string job_id);
	int Camera_Get_Min_Sharpness_Value(const std::string job_id);
	int Camera_Get_Max_Sharpness_Value(const std::string job_id);
	int Camera_Set_Manual_Contrast_Value(const std::string job_id, const int value);
	int Camera_Get_Manual_Contrast_Value(const std::string job_id);
	int Camera_Get_Min_Contrast_Value(const std::string job_id);
	int Camera_Get_Max_Contrast_Value(const std::string job_id);
	int Camera_Set_Manual_ISO_Value(const std::string job_id, const int value);
	int Camera_Get_Manual_ISO_Value(const std::string job_id);
	int Camera_Get_Min_ISO_Value(const std::string job_id);
	int Camera_Get_Max_ISO_Value(const std::string job_id);
	int Camera_Set_Manual_Shutter_Speed_Value(const std::string job_id, const int value);
	int Camera_Get_Manual_Shutter_Speed_Value(const std::string job_id);
	int Camera_Get_Min_Shutter_Speed_Value(const std::string job_id);
	int Camera_Get_Max_Shutter_Speed_Value(const std::string job_id);
	int Camera_Set_Flip_V(const std::string job_id, const bool flip) ;
	int Camera_Get_Flip_V(const std::string job_id) ;
	int Camera_Set_Flip_H(const std::string job_id, const bool flip) ;
	int Camera_Get_Flip_H(const std::string job_id) ;
	int Camera_Set_Factory_Reset(const std::string job_id);
	int Camera_Set_Config_Run(const std::string job_id);

    int Camera_Capture_SW_Trigger(void);
	int Camera_Capture_HW_Trigger(void);
	int Camera_Get_Frame_Count();	
	int Camera_Set_Camera_Image_To_Past_Frame(int FrameNum);

    int Camera_Save_Image_To_Device_Local(const std::string file_name);


    int Digital_IO_GetIn(void);
    int Digital_IO_SetOut(int pin_num, int pin_val);

	std::string GetToolList(void) ;
    std::string GetToolTypeName(const int type) ;
	int Get_Type(const std::string id) ;
	
	int AddTool(const std::string parent_id, const int tool_type) ;
	int InsertTool(const std::string parent_id, const int index, const int tool_type) ;
	int MoveTool(const std::string parent_id, const int cur_index, const int target_index) ;
	int DelTool(const std::string tool_id) ;

	//Tool Option
	int OptionGetImage(const std::string option_id, const int type_option, int& width, int& height, ImageBuf* out_buf) ;

	//Tool Option : Inspect Color
	int Tool_Option_InspectColor_Histogram_GetImage(const std::string option_id, const int color_num, const int type_option, int& width, int& height, ImageBuf* out_buf) ;
	int Tool_Option_InspectColor_Set_Histogram_UseElement(const std::string option_id, const int color_elem) ;
	int Tool_Option_InspectColor_Get_Histogram_UseElement(const std::string option_id) ;
	int Tool_Option_InspectColor_Set_Histogram_Range(const std::string option_id, const int color_elem, const float min, const float max) ;
	int Tool_Option_InspectColor_Get_Histogram_Range(const std::string option_id, const int color_elem, int* out_min, int* out_max) ;
	int Tool_Option_InspectColor_Get_Base_Pixel_Count(const std::string option_id) ;
	int Tool_Option_InspectColor_Get_Tolerance(const std::string option_id, int* out_min, int* out_max) ;
	int Tool_Option_InspectColor_Set_Tolerance(const std::string option_id, const int min, const int max) ;

	//Tool Option : Inspect Distance
	int Tool_Option_InspectDistance_Get_Tolerance(const std::string option_id, float* out_min, float* out_max) ;
	int Tool_Option_InspectDistance_Set_Tolerance(const std::string option_id, const float min, const float max) ;
	std::string Tool_Option_InspectDistance_Get_ID_Info_Base(const std::string option_id) ;
	std::string Tool_Option_InspectDistance_Get_ID_Info_Target(const std::string option_id) ;

	//Tool Option : Inspect Angle
	int Tool_Option_InspectAngle_Get_Tolerance(const std::string option_id, float* out_min, float* out_max) ;
	int Tool_Option_InspectAngle_Set_Tolerance(const std::string option_id, const float min, const float max) ;
	std::string Tool_Option_InspectAngle_Get_ID_Info_Base(const std::string option_id) ;
	std::string Tool_Option_InspectAngle_Get_ID_Info_Target(const std::string option_id) ;

	//Tool Option : Inspect Diameter
	float Tool_Circle_Get_CalcDiameter(const std::string tool_id) ;
	int Tool_Option_InspectDiameter_Get_Tolerance(const std::string option_id, float* out_min, float* out_max) ;
	int Tool_Option_InspectDiameter_Set_Tolerance(const std::string option_id, const float min, const float max) ;
	
	////
	int ToolGetImage(const std::string tool_id, const int type_option, int& width, int& height, ImageBuf* out_buf) ;
	std::string ToolGetName(const std::string tool_id) ;
	int ToolSetName(const std::string tool_id, const std::string name) ;

	int ToolSetMaskArea(const std::string tool_id, float x, float y, float w, float h, bool inverse);
	int ToolUndoMaskArea(const std::string tool_id);
	int ToolDelMaskArea(const std::string tool_id);
	int ToolGetFeatureLevel(const std::string tool_id) ;
	int ToolSetFeatureLevel(const std::string tool_id, const int level) ;	
	int ToolGetUseCustomFeatureOption(const std::string id);
	int ToolSetUseCustomFeatureOption(const std::string id, const bool b_use);
	int Tool_Get_Feature_Option(const std::string id, int* out_param1, int* out_param2, int* out_param3, int* out_param4);
	int Tool_Set_Feature_Option(const std::string id, const int param1, const int param2, const int param3, const int param4);
	
	//crack
	int Tool_Option_Crack_GetInspectLevel(const std::string option_id) ;
	int Tool_Option_Crack_SetInspectLevel(const std::string option_id, const int level) ;

	//color compare
	int Tool_Option_ColorCompare_GetInspectLevel(const std::string option_id) ;
	int Tool_Option_ColorCompare_SetInspectLevel(const std::string option_id, const int level) ;
	int Tool_Option_ColorCompare_GetSensitivity(const std::string option_id) ;
	int Tool_Option_ColorCompare_SetSensitivity(const std::string option_id, const int level) ;

	//object
	int ToolSelectObject(const std::string tool_id, const float x, const float y, const float width, const float height, const int margin=0) ;
	int ToolSelectObject(const std::string tool_id, const float left_top_x, const float left_top_y, const float right_top_x, const float right_top_y, const float right_bottom_x, const float right_bottom_y, const float left_bottom_x, const float left_bottom_y, const int margin=0 )  ;
	int ToolGetObjectImage(const std::string tool_id, const int type_option, int& width, int& height, ImageBuf* out_buf) ;
	int ToolSetRefPoint(const std::string id, const float x, const float y) ;
	int ToolDelRefPoint(const std::string id) ;
	
	//object detct
	int ToolSetDetectOption(const std::string tool_id, const int option, const float value) ;
	float ToolGetDetectOption(const std::string tool_id, const int option) ;	
	int Tool_Detect_Object_Get_Inspection_Tolerance_Info(const std::string tool_id, float* out_score_threshold) ;
	int Tool_Detect_Object_Set_Inspection_Tolerance_Info(const std::string tool_id, const float score_threshold) ;

	//detect code
	std::string Tool_Detect_Code_Get_Ref_CodeType(const std::string tool_id) ;
	std::string Tool_Detect_Code_Get_Ref_CodeData(const std::string tool_id) ;
	float Tool_Detect_Code_Get_Padding(const std::string tool_id) ;
	int Tool_Detect_Code_Set_Padding(const std::string tool_id, const float padding_rate) ;

	//Detect line
	int Tool_Detect_Line_Set_SelectObject(const std::string tool_id, const float line1_x, const float line1_y, const float line2_x, const float line2_y, const float left_top_x, const float left_top_y, const float right_top_x, const float right_top_y, const float right_bottom_x, const float right_bottom_y, const float left_bottom_x, const float left_bottom_y) ;
	
	std::string ToolGetOptionList(const std::string tool_id) ;
	std::string ToolGetOptionList(const int tool_type) ;
	int ToolGetOptionCount(const int tool_type) ;
	int ToolAddNewOption(const std::string tool_id, const int option_type) ;
	int ToolDelOption(const std::string option_id) ;

	//Tool Offset Distance
	int Tool_Offset_Distance_Set_Direction(const std::string tool_id, const int direction) ;
	int Tool_Offset_Distance_Get_Direction(const std::string tool_id) ;
	int Tool_Offset_Distance_Set_Region(const std::string tool_id, const float x, const float y, const float width, const float height) ;
	int Tool_Offset_Distance_Get_Region(const std::string tool_id, float* out_x, float* out_y, float* out_width, float* out_height) ;
	int Tool_Offset_Distance_Get_Inspection_Base_Info(const std::string tool_id, float* out_distance, float *out_angle) ;
	int Tool_Offset_Distance_Get_Inspection_Tolerance_Info(const std::string tool_id, float* out_distance_min, float *out_distance_max, float* out_angle_min, float *out_angle_max)  ;
	int Tool_Offset_Distance_Set_Inspection_Tolerance_Info(const std::string tool_id, const float distance_min, const float distance_max, const float angle_min, const float angle_max)  ;

	std::string Update_Get_Version_List(void);
	std::string Update_Get_Current_Version(void);
	int Update_Set_Version(std::string version_string);
	//////////////////
};

#endif
