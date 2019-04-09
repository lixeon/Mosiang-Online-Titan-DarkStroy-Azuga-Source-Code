namespace web.admin
{
    using System;
    using System.Data;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpcharacter : Page
    {
        protected Button btnedit;
        protected Button btnsearch;
        protected Button btnsearchguild;
        protected Button btnsearuseridx;
        protected DataGrid DataGrid1;
        protected DropDownList ddmap;
        protected DropDownList ddsex;
        protected DropDownList ddstage;
        private static DataSet ds;
        private static DataSet mapds;
        protected static int rcount;
        private static string sql;
        protected TextBox tbabilityexp;
        protected TextBox tbbadfame;
        protected TextBox tbchaidx;
        protected TextBox tbchalv;
        protected TextBox tbchaname;
        protected TextBox tbchareset;
        protected TextBox tbdex;
        protected TextBox tbgengoal;
        protected TextBox tbguild;
        protected TextBox tblogouttime;
        protected TextBox tbmoney;
        protected TextBox tbnewname;
        protected TextBox tbparty;
        protected TextBox tbplaytime;
        protected TextBox tbpoint;
        protected TextBox tbschalvmax;
        protected TextBox tbschalvmin;
        protected TextBox tbschaname;
        protected TextBox tbschapointmax;
        protected TextBox tbschapointmin;
        protected TextBox tbscharesetmax;
        protected TextBox tbscharesetmin;
        protected TextBox tbsimmak;
        protected TextBox tbsta;
        protected TextBox tbsuserid;
        protected TextBox tbuseridx;

        private void btnedit_Click(object sender, EventArgs e)
        {
            int num = int.Parse(this.tbchaidx.Text.ToString());
            string str = new system().ConvertToBig5(new system().ChkSql(this.tbchaname.Text.ToString()), 0x3a8);
            int num2 = int.Parse(this.ddsex.SelectedValue.ToString());
            int num3 = int.Parse(this.ddstage.SelectedValue.ToString());
            int num4 = int.Parse(this.ddmap.SelectedValue.ToString());
            int num5 = int.Parse(this.tbchalv.Text.ToString());
            double num6 = double.Parse(this.tbmoney.Text.ToString());
            int num7 = int.Parse(this.tbchareset.Text.ToString());
            int num8 = int.Parse(this.tbnewname.Text.ToString());
            int num9 = int.Parse(this.tbgengoal.Text.ToString());
            int num10 = int.Parse(this.tbsta.Text.ToString());
            int num11 = int.Parse(this.tbdex.Text.ToString());
            int num12 = int.Parse(this.tbsimmak.Text.ToString());
            int num13 = int.Parse(this.tbpoint.Text.ToString());
            int num14 = int.Parse(this.tbabilityexp.Text.ToString());
            int num15 = int.Parse(this.tbbadfame.Text.ToString());
            int num16 = int.Parse(this.tbplaytime.Text.ToString());
            string mySql = string.Concat(new object[] { 
                "UPDATE [MHGAME].[dbo].[TB_CHARACTER] SET  [CHARACTER_name]='", str, "',[CHARACTER_abilityexp]=", num14, ",[CHARACTER_badfame]=", num15, ",[CHARACTER_GENDER]=", num2, ", [CHARACTER_gengoal]=", num9, ", [CHARACTER_DEX]=", num11, ", [CHARACTER_STA]=", num10, ", [CHARACTER_SIMMAK]=", num12, 
                ",  [CHARACTER_STAGE]=", num3, ", [CHARACTER_GRADE]=", num5, ", [CHARACTER_GRADEUPPOINT]=", num13, ", [CHARACTER_MONEY]=", num6, ", [CHARACTER_MAP]=", num4, ", [CHARACTER_PLAYTIME]=", num16, ", [WebChaReset]=", num7, ", [WebNewName]=", num8, 
                " WHERE character_idx=", num
             });
            new DataProviders().ExecuteSql(mySql);
        }

        private void btnsearch_Click(object sender, EventArgs e)
        {
            system system = new system();
            string str = system.ChkSql(this.tbsuserid.Text.ToString().Trim());
            string str2 = system.ChkSql(this.tbschaname.Text.ToString().Trim());
            int num = int.Parse(this.tbschalvmin.Text.ToString().Trim());
            int num2 = int.Parse(this.tbschalvmax.Text.ToString().Trim());
            int num3 = int.Parse(this.tbschapointmin.Text.ToString());
            int num4 = int.Parse(this.tbschapointmax.Text.ToString());
            int num5 = int.Parse(this.tbscharesetmin.Text.ToString());
            int num6 = int.Parse(this.tbscharesetmax.Text.ToString());
            sql = "select * from mhgame..tb_character  where substring(character_name,1,1)!='@'";
            string str3 = "";
            if (str != "")
            {
                str3 = str3 + " and user_idx in(select propid from mhcmember..chr_log_info where id_loginid like '%" + str + "%')";
            }
            if (str2 != "")
            {
                str2 = new system().ConvertToBig5(str2, 0x3a8);
                str3 = str3 + " and character_name like '%" + str2 + "%'";
            }
            if (num != 0)
            {
                str3 = str3 + " and character_grade >= " + num;
            }
            if (num2 != 0)
            {
                str3 = str3 + " and character_grade <= " + num2;
            }
            if (num3 != 0)
            {
                str3 = str3 + " and Character_gengoal+Character_STA+Character_DEX+Character_SIMMAK >= " + num3;
            }
            if (num4 != 0)
            {
                str3 = str3 + " and Character_gengoal+Character_STA+Character_DEX+Character_SIMMAK <= " + num4;
            }
            if (num5 != 0)
            {
                str3 = str3 + " and webchareset >= " + num5;
            }
            if (num6 != 0)
            {
                str3 = str3 + " and webchareset <= " + num6;
            }
            sql = sql + str3;
            ds = new DataProviders().ExecuteSqlDs(sql, "DataGrid1");
            for (int i = 0; i < ds.Tables[0].Rows.Count; i++)
            {
                ds.Tables[0].Rows[i]["character_name"] = system.ConvertToBig5(ds.Tables[0].Rows[i]["character_name"].ToString(), 950);
            }
            this.DataGrid1.DataSource = ds;
            this.DataGrid1.CurrentPageIndex = 0;
            this.DataGrid1.DataBind();
            rcount = ds.Tables[0].Rows.Count;
        }

        private void btnsearuseridx_Click(object sender, EventArgs e)
        {
            base.Response.Write("<script>window.open('cpmember.aspx?useridx=" + this.tbuseridx.Text.ToString() + "')</script>");
        }

        private void DataGrid1_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid1.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid1.DataSource = ds;
            this.DataGrid1.DataBind();
        }

        private void DataGrid1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int num = int.Parse(this.DataGrid1.SelectedItem.Cells[0].Text.ToString());
            DataProviders providers = new DataProviders();
            SqlDataReader reader = providers.ExecuteSqlDataReader("select * from mhgame..tb_character  where character_idx=" + num);
            if (reader.Read())
            {
                this.tbuseridx.Text = reader["user_idx"].ToString();
                this.tbchaidx.Text = reader["character_idx"].ToString();
                this.tbchaname.Text = new system().ConvertToBig5(reader["character_name"].ToString(), 950);
                this.tbguild.Text = new system().ConvertToBig5(reader["character_munpaname"].ToString(), 950);
                this.tbparty.Text = reader["character_partyidx"].ToString();
                if (this.tbguild.Text.ToString() == "")
                {
                    this.btnsearchguild.Enabled = false;
                }
                else
                {
                    this.btnsearchguild.Enabled = true;
                }
                this.ddsex.SelectedValue = reader["character_gender"].ToString();
                this.ddstage.SelectedValue = reader["character_stage"].ToString();
                this.ddmap.SelectedValue = reader["character_map"].ToString();
                this.tbchalv.Text = reader["character_grade"].ToString();
                this.tbmoney.Text = reader["character_money"].ToString();
                this.tbchareset.Text = reader["webchareset"].ToString();
                this.tbnewname.Text = reader["webnewname"].ToString();
                this.tbgengoal.Text = reader["character_gengoal"].ToString();
                this.tbsta.Text = reader["character_sta"].ToString();
                this.tbdex.Text = reader["character_dex"].ToString();
                this.tbsimmak.Text = reader["character_simmak"].ToString();
                this.tbpoint.Text = reader["character_gradeuppoint"].ToString();
                this.tbabilityexp.Text = reader["character_abilityexp"].ToString();
                this.tbbadfame.Text = reader["character_badfame"].ToString();
                this.tblogouttime.Text = reader["character_lastmodified"].ToString();
                this.tbplaytime.Text = reader["character_playtime"].ToString();
            }
            reader.Close();
            providers.CloseConn();
        }

        private void InitializeComponent()
        {
            this.btnsearch.Click += new EventHandler(this.btnsearch_Click);
            this.DataGrid1.PageIndexChanged += new DataGridPageChangedEventHandler(this.DataGrid1_PageIndexChanged);
            this.DataGrid1.SelectedIndexChanged += new EventHandler(this.DataGrid1_SelectedIndexChanged);
            this.btnsearuseridx.Click += new EventHandler(this.btnsearuseridx_Click);
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new WebLogic().isadmin();
            new WebLogic().diskf();
            if (base.Request.QueryString["userid"] != null)
            {
                this.tbsuserid.Text = base.Request.QueryString["userid"].ToString();
            }
            if (!this.Page.IsPostBack)
            {
                mapds = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_map_bin", "DataGrid1");
                this.ddmap.DataSource = mapds;
                this.ddmap.DataTextField = "name";
                this.ddmap.DataValueField = "id";
                this.ddmap.DataBind();
            }
        }
    }
}
