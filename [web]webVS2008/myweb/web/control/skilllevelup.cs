namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class skilllevelup : UserControl
    {
        protected Button btnlvup;
        protected DropDownList ddchalist;
        protected DropDownList ddmugong;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator Requiredfieldvalidator6;

        private void btnlvup_Click(object sender, EventArgs e)
        {
            int chaidx = int.Parse(this.ddchalist.SelectedValue.ToString());
            int mugongidx = int.Parse(this.ddmugong.SelectedValue.ToString());
            int needmoney = int.Parse(base.Application["game.skilllvupmoney"].ToString());
            string str = new WebLogic().skilllevelup(base.Session["userid"].ToString(), chaidx, mugongidx, needmoney);
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void ddchalist_SelectedIndexChanged(object sender, EventArgs e)
        {
            int num = int.Parse(this.ddchalist.SelectedValue);
            if (num == 0)
            {
                this.ddmugong.Items.Clear();
            }
            else
            {
                this.ddmugong.DataSource = new DataProviders().ExecuteSqlDs("select b.name+'['+convert(varchar,a.mugong_sung)+'層]' as name,a.mugong_dbidx  from mhgame..tb_mugong a,mhcmember..web_skilllist_bin b where a.mugong_idx = b.id and character_idx=" + num, "ddmugong");
                this.ddmugong.DataTextField = "name";
                this.ddmugong.DataValueField = "mugong_dbidx";
                this.ddmugong.DataBind();
            }
        }

        private void InitializeComponent()
        {
            this.ddchalist.SelectedIndexChanged += new EventHandler(this.ddchalist_SelectedIndexChanged);
            this.btnlvup.Click += new EventHandler(this.btnlvup_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            if (base.Session["userid"] == null)
            {
                base.Response.Redirect("login.aspx?returnurl=" + base.Request.ServerVariables["SCRIPT_NAME"]);
            }
            else if (!this.Page.IsPostBack)
            {
                this.ddchalist.DataSource = new DataProviders().ExecuteSqlDs("select character_idx,character_name+'['+convert(varchar,character_money)+'笴栏刽]' as character_name from mhgame..tb_character  where substring(character_name,1,1)!='@' and user_idx=" + base.Session["useridx"].ToString(), "ddchalist");
                this.ddchalist.DataTextField = "character_name";
                this.ddchalist.DataValueField = "character_idx";
                this.ddchalist.DataBind();
                for (int i = 0; i < this.ddchalist.Items.Count; i++)
                {
                    this.ddchalist.Items[i].Text = new system().ConvertToBig5(this.ddchalist.Items[i].Text.ToString(), 950);
                }
                this.ddchalist.Items.Insert(0, new ListItem("請選擇角色", "0"));
            }
        }
    }
}
