#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
#include <direct.h>
#include <stdlib.h>
#include <string>
#include ".\include\json\json.h"
#include ".\include\json\value.h"
#include ".\include\json\writer.h"
#include <msclr\marshal_cppstd.h>
using namespace msclr::interop;

namespace Excel2JSON {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Runtime::InteropServices;
	namespace Excel = Microsoft::Office::Interop::Excel;	// call Excel

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class frmMain : public System::Windows::Forms::Form
	{
	public:
		frmMain(void)
		{
			InitializeComponent();
			//
			//TODO:  在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~frmMain()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ToolStrip^ toolStrip1;
	protected:
	private: System::Windows::Forms::ToolStripLabel^ toolStripLabel1;
	private: System::Windows::Forms::ToolStripComboBox^ tcboExcelFolder;
	private: System::Windows::Forms::ToolStripButton^ tbtnBrowse;
	private: System::Windows::Forms::StatusStrip^ statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^ toolStripStatusLabel1;
	private: System::Windows::Forms::ToolStripStatusLabel^ stlsMessage;

	private: System::Windows::Forms::SplitContainer^ splitContainer1;
	private: System::Windows::Forms::ListBox^ lstFile;
	private: System::Windows::Forms::DataGridView^ gridData;
	private: System::Windows::Forms::Button^ Convert;

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(frmMain::typeid));
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripLabel1 = (gcnew System::Windows::Forms::ToolStripLabel());
			this->tcboExcelFolder = (gcnew System::Windows::Forms::ToolStripComboBox());
			this->tbtnBrowse = (gcnew System::Windows::Forms::ToolStripButton());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->stlsMessage = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->lstFile = (gcnew System::Windows::Forms::ListBox());
			this->gridData = (gcnew System::Windows::Forms::DataGridView());
			this->Convert = (gcnew System::Windows::Forms::Button());
			this->toolStrip1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->gridData))->BeginInit();
			this->SuspendLayout();
			// 
			// toolStrip1
			// 
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->toolStripLabel1,
					this->tcboExcelFolder, this->tbtnBrowse
			});
			this->toolStrip1->Location = System::Drawing::Point(0, 0);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(867, 25);
			this->toolStrip1->TabIndex = 0;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// toolStripLabel1
			// 
			this->toolStripLabel1->Name = L"toolStripLabel1";
			this->toolStripLabel1->Size = System::Drawing::Size(149, 22);
			this->toolStripLabel1->Text = L"Select Excel File Directory";
			// 
			// tcboExcelFolder
			// 
			this->tcboExcelFolder->AutoSize = false;
			this->tcboExcelFolder->Name = L"tcboExcelFolder";
			this->tcboExcelFolder->Size = System::Drawing::Size(400, 23);
			this->tcboExcelFolder->TextUpdate += gcnew System::EventHandler(this, &frmMain::tcboExcelFolder_TextUpdate);
			// 
			// tbtnBrowse
			// 
			this->tbtnBrowse->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tbtnBrowse->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"tbtnBrowse.Image")));
			this->tbtnBrowse->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tbtnBrowse->Name = L"tbtnBrowse";
			this->tbtnBrowse->Size = System::Drawing::Size(23, 22);
			this->tbtnBrowse->Text = L"Directory explore";
			this->tbtnBrowse->Click += gcnew System::EventHandler(this, &frmMain::tbtnBrowse_Click);
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->toolStripStatusLabel1,
					this->stlsMessage
			});
			this->statusStrip1->Location = System::Drawing::Point(0, 410);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(867, 22);
			this->statusStrip1->TabIndex = 1;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(67, 17);
			this->toolStripStatusLabel1->Text = L"Message : ";
			// 
			// stlsMessage
			// 
			this->stlsMessage->AutoSize = false;
			this->stlsMessage->Name = L"stlsMessage";
			this->stlsMessage->Size = System::Drawing::Size(400, 17);
			this->stlsMessage->Text = L"No Message.";
			this->stlsMessage->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 25);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->lstFile);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->gridData);
			this->splitContainer1->Size = System::Drawing::Size(867, 385);
			this->splitContainer1->SplitterDistance = 289;
			this->splitContainer1->TabIndex = 2;
			// 
			// lstFile
			// 
			this->lstFile->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->lstFile->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lstFile->FormattingEnabled = true;
			this->lstFile->ItemHeight = 12;
			this->lstFile->Location = System::Drawing::Point(0, 0);
			this->lstFile->Name = L"lstFile";
			this->lstFile->Size = System::Drawing::Size(289, 385);
			this->lstFile->TabIndex = 0;
			this->lstFile->DoubleClick += gcnew System::EventHandler(this, &frmMain::lstFile_DoubleClick);
			// 
			// gridData
			// 
			this->gridData->BackgroundColor = System::Drawing::SystemColors::ButtonHighlight;
			this->gridData->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->gridData->Dock = System::Windows::Forms::DockStyle::Fill;
			this->gridData->Location = System::Drawing::Point(0, 0);
			this->gridData->Name = L"gridData";
			this->gridData->RowTemplate->Height = 24;
			this->gridData->Size = System::Drawing::Size(574, 385);
			this->gridData->TabIndex = 0;
			// 
			// Convert
			// 
			this->Convert->Location = System::Drawing::Point(650, 0);
			this->Convert->Name = L"Convert";
			this->Convert->Size = System::Drawing::Size(167, 25);
			this->Convert->TabIndex = 3;
			this->Convert->Text = L"Convert";
			this->Convert->UseVisualStyleBackColor = true;
			this->Convert->Click += gcnew System::EventHandler(this, &frmMain::Convert_Click);
			// 
			// frmMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(867, 432);
			this->Controls->Add(this->Convert);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->toolStrip1);
			this->Name = L"frmMain";
			this->Text = L"Excel To JSON Ver.1";
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->gridData))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void tbtnBrowse_Click(System::Object^ sender, System::EventArgs^ e) {
		stlsMessage->Text = L"Selecting Folder...";
		FolderBrowserDialog^ dlg = gcnew FolderBrowserDialog();
		dlg->SelectedPath = tcboExcelFolder->Text;
		if (dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			lstFile->Items->Clear();
			tcboExcelFolder->Text = dlg->SelectedPath;
			FileInfo^ currentFile;
			DirectoryInfo^ di = gcnew DirectoryInfo(tcboExcelFolder->Text);
			array<FileInfo^>^ files = di->GetFiles("*", SearchOption::TopDirectoryOnly);
			for each (FileInfo ^ file in files)
			{
				currentFile = file;
				String^ fCurrentName = String::Format("{0}", currentFile);
				lstFile->Items->Add(fCurrentName);
			}
		};
		delete dlg;
		if (tcboExcelFolder->Text == "")
		{
			stlsMessage->Text = L"Please select folder first.";
		}
		else 
		{
			stlsMessage->Text = L"Please select Excel file from left list.";
		}
	}
private: bool isExcelFile(String^ text) {
		int dotIdx = 0;
		while (text[dotIdx] != '.')
		{
			dotIdx++;
		};
		String^ ext = lstFile->Text->Substring(dotIdx + 1);
		if (ext == "xlsx" || ext == "xlsm" || ext == "xlsb" || ext == "xltx" || ext == "xltm" )
		{
			return true;
		};
		if (ext == "xls" || ext == "xlt" || ext == "xml" || ext == "xlam" )
		{
			return true;
		};
		if (ext == "csv" || ext == "xla" || ext == "xlw" || ext == "xlr")
		{
			return true;
		};
		return false;
}
private: System::Void lstFile_DoubleClick(System::Object^ sender, System::EventArgs^ e) {
		if (!isExcelFile(lstFile->Text))
		{
			stlsMessage->Text = L"Not Excel file.";
			return;
		};
		stlsMessage->Text = L"Opening Excel file...";
		OpenExcelFile(tcboExcelFolder->Text + "\\" + lstFile->Text);
		stlsMessage->Text = L"Display Excel file complete.";
	}
private: void OpenExcelFile(String^ strXlsFile) {
		String^ strXlsPath = strXlsFile;
		Excel::Application^ ExcelApp;
		//Excel::Workbook^ ExcelBook;
		ExcelApp = gcnew Excel::Application();
		ExcelApp->Visible = false;
		Excel::Workbook^ ExcelWb = ExcelApp->Workbooks->Add(strXlsPath);
		Excel::Worksheet^ ExcelSheet = (Excel::Worksheet^)ExcelApp->Workbooks[1]->Worksheets[1];
		
		gridData->Rows->Clear();
		gridData->ColumnCount = 9;
		gridData->RowHeadersWidth = 5;
		gridData->Columns[0]->Width = 200;
		gridData->Columns[1]->Width = 200;
		for (int i = 2; i < 9; i++)
		{
			gridData->Columns[i]->Width = 80;
		};

		int i = 1;
		Microsoft::Office::Interop::Excel::Range^ range;
		range = (Excel::Range^)ExcelSheet->Cells[1, 1];
		String^ strCell = range->Text->ToString();

		while (strCell->Length > 0)
		{
			if (i == 1)
			{
				for (int j = 0; j < 9; j++)
				{
					range = (Excel::Range^)ExcelSheet->Cells[1, j + 1];
					gridData->Columns[j]->Name = range->Text->ToString();
				}
			}
			else
			{
				gridData->Rows->Add();
				for (int j = 1; j < 10; j++)
				{
					range = (Excel::Range^)ExcelSheet->Cells[i, j];
					gridData->Rows[i - 2]->Cells[j - 1]->Value = range->Text->ToString();
				};
			};
			i++;
			range = (Excel::Range^)ExcelSheet->Cells[i, 1];
			strCell = range->Text->ToString();
		};
		Marshal::ReleaseComObject(range);
		Marshal::ReleaseComObject(ExcelSheet);
		Marshal::ReleaseComObject(ExcelWb);
		ExcelApp->Quit();
		Marshal::ReleaseComObject(ExcelApp);
	}
private: System::Void tcboExcelFolder_TextUpdate(System::Object^ sender, System::EventArgs^ e) {
		lstFile->Items->Clear();
		FileInfo^ currentFile;
		DirectoryInfo^ di = gcnew DirectoryInfo(tcboExcelFolder->Text);
		array<FileInfo^>^ files = di->GetFiles("*", SearchOption::TopDirectoryOnly);
		for each (FileInfo ^ file in files)
		{
			currentFile = file;
			String^ fCurrentName = String::Format("{0}", currentFile);
			lstFile->Items->Add(fCurrentName);
		};
		stlsMessage->Text = L"Please select Excel file from left list.";
	}
private: System::Void Convert_Click(System::Object^ sender, System::EventArgs^ e) {
		if (tcboExcelFolder->Text == "")
		{
			stlsMessage->Text = L"Please select folder first.";
			return;
		};
		if (!isExcelFile(lstFile->Text)) {
			stlsMessage->Text = L"Please select Excel file.";
			return;
		};
		stlsMessage->Text = L"Converting Excel To JSON...";
		dataToJSON(tcboExcelFolder->Text + "\\" + lstFile->Text);
		stlsMessage->Text = L"Convert complete.";
	}
private: void dataToJSON(String^ strXlsFile) {
		Json::Value ServerData;
		Json::Value ClientData;
		std::string sfile, cfile;
		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "None";
		//builder.settings_["emitUTF8"] = true;
		builder["indentation"] = "   ";
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		String^ strXlsPath = strXlsFile;
		Excel::Application^ ExcelApp;
		//Excel::Workbook^ ExcelBook;
		ExcelApp = gcnew Excel::Application();
		ExcelApp->Visible = false;
		Excel::Workbook^ ExcelWb = ExcelApp->Workbooks->Add(strXlsPath);
		Excel::Worksheet^ ExcelSheet = (Excel::Worksheet^)ExcelApp->Workbooks[1]->Worksheets[1];
		std::string SheetName = marshal_as<std::string>(ExcelSheet->Name);
		std::string PathName = marshal_as<std::string>(tcboExcelFolder->Text);

		int row = 1;
		Microsoft::Office::Interop::Excel::Range^ range;
		range = (Excel::Range^)ExcelSheet->Cells[1, 1];
		String^ strCell = range->Text->ToString();

		while (strCell->Length > 0)
		{
			row++;
			range = (Excel::Range^)ExcelSheet->Cells[row, 1];
			strCell = range->Text->ToString();
		};
		for (int i = 5; i < row; i++)
		{
			range = (Excel::Range^)ExcelSheet->Cells[i, 2]; // open
			strCell = range->Text->ToString();
			if (strCell == "1")
			{
				Json::Value sMetaData;
				Json::Value cMetaData;
				std::string key, val;
				for (int j = 1; j <= 9; j++)
				{
					if (j == 2) // open not show
					{
						continue;
					};
					range = (Excel::Range^)ExcelSheet->Cells[4, j]; // server or client or ps
					strCell = range->Text->ToString();
					if (strCell == "S")
					{
						range = (Excel::Range^)ExcelSheet->Cells[2, j]; // key
						key = marshal_as<std::string>(range->Text->ToString());
						range = (Excel::Range^)ExcelSheet->Cells[i, j]; // value
						val = marshal_as<std::string>(range->Text->ToString());
						sMetaData[key] = val;
					}
					else if (strCell == "C")
					{
						range = (Excel::Range^)ExcelSheet->Cells[2, j]; // key
						key = marshal_as<std::string>(range->Text->ToString());
						range = (Excel::Range^)ExcelSheet->Cells[i, j]; // value
						val = marshal_as<std::string>(range->Text->ToString());
						cMetaData[key] = val;
					}
					else if (strCell == "CS")
					{
						range = (Excel::Range^)ExcelSheet->Cells[2, j]; // key
						key = marshal_as<std::string>(range->Text->ToString());
						range = (Excel::Range^)ExcelSheet->Cells[i, j]; // value
						val = marshal_as<std::string>(range->Text->ToString());
						sMetaData[key] = val;
						cMetaData[key] = val;
					};
				};
				ServerData.append(sMetaData);
				ClientData.append(cMetaData);
			};
		};
		sfile = PathName + "\\" + SheetName + "server.json";
		cfile = PathName + "\\" + SheetName + "client.json";
		std::ofstream soutputFileStream(sfile);
		std::ofstream coutputFileStream(cfile);
		writer->write(ServerData, &soutputFileStream);
		writer->write(ClientData, &coutputFileStream);
		Marshal::ReleaseComObject(range);
		Marshal::ReleaseComObject(ExcelSheet);
		Marshal::ReleaseComObject(ExcelWb);
		ExcelApp->Quit();
		Marshal::ReleaseComObject(ExcelApp);
	};
};
}
