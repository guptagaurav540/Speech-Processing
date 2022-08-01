#pragma once
#include "stdAfx.h"
#include "success.h"
#include "hmm.h"
#include <iostream>
#include <cstdlib>
#include<ctime>

//generating random number
int num = 0;
char* mydata[] = {"zero","one","two","three","four","five","six","seven","eight","nine","google","apple","ola","hello","okay"};
namespace captcha {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected: 
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::PictureBox^  pictureBox1;

	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Label^  label1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::Blue;
			this->button1->Enabled = false;
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->button1->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->button1->Location = System::Drawing::Point(169, 423);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(115, 52);
			this->button1->TabIndex = 0;
			this->button1->Text = L"LogIn";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::SystemColors::InactiveBorder;
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Magneto", 16));
			this->textBox1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(128)), 
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->textBox1->Location = System::Drawing::Point(37, 41);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(371, 40);
			this->textBox1->TabIndex = 1;
			this->textBox1->Text = L"Enter Email Id.";
			this->textBox1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox2
			// 
			this->textBox2->AllowDrop = true;
			this->textBox2->BackColor = System::Drawing::SystemColors::InactiveBorder;
			this->textBox2->Font = (gcnew System::Drawing::Font(L"Magneto", 16));
			this->textBox2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(128)), 
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->textBox2->Location = System::Drawing::Point(37, 101);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(371, 40);
			this->textBox2->TabIndex = 2;
			this->textBox2->Text = L"Enter Roll Number.";
			this->textBox2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::Transparent;
			Image^ i1 = Image::FromFile(L"0.jpg");
			this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(i1));
			this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox1->InitialImage = nullptr;
			this->pictureBox1->Location = System::Drawing::Point(144, 198);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(149, 68);
			this->pictureBox1->TabIndex = 3;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Click += gcnew System::EventHandler(this, &Form1::pictureBox1_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoCheck = false;
			this->checkBox1->AutoSize = true;
			this->checkBox1->BackColor = System::Drawing::SystemColors::InactiveBorder;
			this->checkBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->checkBox1->ForeColor = System::Drawing::Color::Red;
			this->checkBox1->Location = System::Drawing::Point(92, 360);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(175, 29);
			this->checkBox1->TabIndex = 5;
			this->checkBox1->Text = L"I am not a Robot";
			this->checkBox1->UseVisualStyleBackColor = false;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox1_CheckedChanged);
			// 
			// button2
			// 
			this->button2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"button2.Image")));
			this->button2->Location = System::Drawing::Point(315, 212);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(55, 43);
			this->button2->TabIndex = 6;
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// button3
			// 
			this->button3->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"button3.Image")));
			this->button3->Location = System::Drawing::Point(315, 285);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(55, 53);
			this->button3->TabIndex = 7;
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->label1->Location = System::Drawing::Point(76, 299);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(233, 25);
			this->label1->TabIndex = 8;
			this->label1->Text = L"Press record and speak...";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label1->Click += gcnew System::EventHandler(this, &Form1::label1_Click);
			// 
			// Form1
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Window;
			this->ClientSize = System::Drawing::Size(442, 553);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F));
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Hide();
			 success^ f2 = gcnew success();
			 f2->ShowDialog();
		 }
private: System::Void textBox3_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 

			 char *name = controller();
			 String ^spoken = gcnew String(name);
			 String ^expected = gcnew String(mydata[num]); //Converted to System::String
			 label1->Text = "Recognised: "+spoken;// ---- main section .....
			 if(expected==spoken){
					checkBox1->Checked = true;
					checkBox1->BackColor = System::Drawing::Color::Red;
					checkBox1->ForeColor = System::Drawing::Color::White;
					checkBox1->Text = L"Successfully Validated Captcha....";
					button1->Enabled = true;
			 }else{
					checkBox1->BackColor = System::Drawing::Color::Red;
					checkBox1->ForeColor = System::Drawing::Color::White;
					checkBox1->Text = L"Try Again....";
			 }
			 
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {

			 // seed
			 srand(time(0));
			 num = (rand() % 15);
			 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));

			 if(num==0){
					Image^ image = Image::FromFile(L"0.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }	
			 else if(num==1){
					Image^ image = Image::FromFile(L"1.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
			 else if(num==2){
					Image^ image = Image::FromFile(L"2.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
			 else if(num==3){
					Image^ image = Image::FromFile(L"3.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
			 else if(num==4){
					Image^ image = Image::FromFile(L"4.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
			 else if(num==5){
					Image^ image = Image::FromFile(L"5.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
			 else if(num==6){
					Image^ image = Image::FromFile(L"6.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
			 else if(num==7){
					Image^ image = Image::FromFile(L"7.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
			 else if(num==8){
					Image^ image = Image::FromFile(L"8.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
			 else if(num==9){
					Image^ image = Image::FromFile(L"9.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
			 else if(num==10){
					Image^ image = Image::FromFile(L"10.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
			 else if(num==11){
					Image^ image = Image::FromFile(L"11.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
			 else if(num==12){
					Image^ image = Image::FromFile(L"12.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
			 else if(num==13){
					Image^ image = Image::FromFile(L"13.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
			 else if(num==14){
					Image^ image = Image::FromFile(L"14.jpg");
					this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(image));
			 }
		 }
private: System::Void pictureBox1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}

